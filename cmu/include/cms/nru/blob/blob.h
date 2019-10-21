#ifndef BLOB_H_
#define BLOB_H_

#include "misc.h"
#include "status.h"
#include "kv_stor.h"
#include "iterator.h"

namespace cdb {

enum BlobType {
  STREAM_BLOB = 1, COMPOUND_BLOB
};

struct BlobWriteOptions {
    // Requires: divisible by 256K.
    uint32 blob_stripe_bytes;

    // RAID
    uint32 data_chunks;
    uint32 parity_chunks;

    // Automatically flush buffered data to chunk server after flush_ms.
    uint32 flush_ms;

    BlobWriteOptions() :
        blob_stripe_bytes(1024 * 1024), data_chunks(8), parity_chunks(1), flush_ms(
            2000) {
    }
};

struct BlobAttr {
    BlobType type;
    uint64 blob_size;
    // start_time and mod_time are invalid for SMALL_BLOB
    uint64 start_time;
    uint64 mod_time;
    bool finalized;
    BlobWriteOptions write_options;
};

struct ChunkLocation {
    uint32 cs_id;
    uint32 disk_id;
};

struct BlobChunkAttr {
    uint32 chunk_id;
    // if cs_id == 0, means unallocated.
    uint32 cs_id;
    // if disk_id == 0, means unallocated.
    uint32 disk_id;
    // For chunk that's stored in multiple disks. empty for chunk that's stored within one disk.
    std::vector<ChunkLocation> extras;
};

class Blob {
  public:
    virtual ~Blob() {
    }

    // key = blob_category/blob_name
    virtual const std::string& key() = 0;

    virtual Status GetAttr(BlobAttr* attr) = 0;

    virtual bool is_finalized() = 0;

    virtual AttachmentIterator* NewAttachmentIterator() = 0;

  protected:
    Blob() {
    }

  private:

    DISALLOW_COPY_AND_ASSIGN(Blob);
};

// Attachement: small object attached to Blob.
// Automatically delete attachments when deleting Blob.
class AppendOnlyBlob : public Blob {
  public:
    AppendOnlyBlob() {
    }
    virtual ~AppendOnlyBlob() {
    }

    // Notify library to flush buffered data to remote chunk servers.
    virtual void Flush() = 0;

    // Once Finalize() is called, append operation is disallowed.
    virtual Status Finalize() = 0;

    // blocked until data is on chunk servre's disk. Need be called after Finalize call.
    // one use case: internal testing
    virtual Status SyncToDisk() = 0;

    // If finalized = true, attachment can't be appended anymore.
    // Requires: one attachment size <= 4MB.
    // attachment_key can't start with '$'.
    virtual Status AppendToAttachment(const std::string& attachment_key,
                                      const char* data, int32 bytes,
                                      bool finalized = false) = 0;

    virtual KVStor* kv_stor() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(AppendOnlyBlob);
};

class ReadOnlyBlob : public Blob {
  public:
    ReadOnlyBlob() {
    }
    virtual ~ReadOnlyBlob() {
    }

    // usage:
    // std::string index_data;
    // Status s = blob->ReadAttachment("video_index", &index_data, 1000, 0);
    // if (s.ok()) {
    //   // use index_data.
    // }
    virtual Status ReadAttachement(const std::string& attachment_key,
                                   std::string* data, uint32 timeout_ms = 0,
                                   uint32 offset = 0) = 0;

    // Key/value Get operation.
    virtual Status Get(const std::string& key, std::string* value) = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(ReadOnlyBlob);
};

class AppendOnlyStreamBlob : public AppendOnlyBlob {
  public:
    virtual ~AppendOnlyStreamBlob() {
    }

    virtual Status Append(const char* data, int32 bytes) = 0;

  protected:
    AppendOnlyStreamBlob() {
    }

  private:
    DISALLOW_COPY_AND_ASSIGN(AppendOnlyStreamBlob);
};

struct BlobReadOptions {
    BlobReadOptions() :
        fill_cache(true), timeout_msecs(0), client_prefetch_blocks(2), chunk_prefetch_blocks(
            2) {
    }

    bool fill_cache;
    // timeout in milli-seconds. 0 means using default timeout.
    uint32 timeout_msecs;

    int client_prefetch_blocks;
    int chunk_prefetch_blocks;
};

class ReadOnlyStreamBlob : public ReadOnlyBlob {
  public:
    virtual ~ReadOnlyStreamBlob() {
    }

    virtual uint32 Read(
        uint64 offset, uint32 bytes, char* dst, Status* status,
        const BlobReadOptions& options = kDefaultReadOptions_) = 0;

  protected:
    ReadOnlyStreamBlob() {
    }

  private:
    static BlobReadOptions kDefaultReadOptions_;

    DISALLOW_COPY_AND_ASSIGN(ReadOnlyStreamBlob);
};

class AppendOnlyCompoundBlob : public AppendOnlyBlob {
  public:
    AppendOnlyCompoundBlob() {
    }

    virtual ~AppendOnlyCompoundBlob() {
    }

    // FIXME: limit size?
    // Return Status::IsBlobTooLarge if current blob size >= 4G.
    // Return Status::IsAttachmentTooLarge if name buffer is too large.
    // Return Status::IsTooManyItems if number of items > FLAGS_max_compound_items.
    virtual Status AppendItem(const std::string& name, const char* data,
                              uint32 size, uint32* item_id) = 0;
  private:
    DISALLOW_COPY_AND_ASSIGN(AppendOnlyCompoundBlob);
};

// Parallel read:
//    ReadOnlyCompoundBlob * blob;
//    Status s;
//    ItemReader* reader1 = blob->ReadItem(1, 100, &s);
//    ItemReader* reader2 = blob->ReadItem(2, 100, &s);
//    // two readers started read in parallel.
//
//    Status s = reader1->Read(buf);
//    if (s.ok()) {
//      // processing data:
//    }
//    s = reader2->Read(buf2);
//    if (s.ok()) {
//      // processing data:
//    }
//
class ItemReader {
  public:
    virtual ~ItemReader() {
    }

    // return item size.
    virtual uint32 size() = 0;

    // Read the whole item to dst.
    // dst need point to buffer at least size() bytes.
    virtual Status Read(char* dst) = 0;

  protected:
    ItemReader() {
    }

  private:
    DISALLOW_COPY_AND_ASSIGN(ItemReader);
};

class ReadOnlyCompoundBlob : public ReadOnlyBlob {
  public:
    ReadOnlyCompoundBlob() {
    }

    virtual ~ReadOnlyCompoundBlob() {
    }

    virtual ItemReader* ReadItem(uint32 item_id, uint32 timeout_ms,
                                 Status* status) = 0;

    // if prefix is empty, return all items.
    virtual ItemIterator* NewIterator(const std::string& prefix) = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(ReadOnlyCompoundBlob);
};

} // namespace

#endif /* BLOB_H_ */
