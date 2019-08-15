// container_key = container_category/container_name
// blob_key = blob_category/blob_name
// blob's File system path: /container_category/container_name/blob_category/blob_name.
// REQUIRES: blob_key need be unique within one container.
//

#ifndef BLOB_STOR_H_
#define BLOB_STOR_H_

#include "misc.h"
#include "status.h"
#include "kv_stor.h"
#include "blob.h"

namespace cdb {

struct ZoneAttr {
    std::string name;
    uint64 used_bytes;
    uint64 available_bytes;
};
struct BlobStorAttr {
    uint64 used_bytes;
    uint64 available_bytes;
    Status status;
    uint32 default_timeout_ms;
    std::vector<ZoneAttr> zone_attrs;
};

struct CategoryAttr {
    uint64 used_bytes;
    // how many blobs.
    uint64 blob_count;
    // how many containers.
    uint64 container_count;
};

struct ContainerAttr {
    // how many blobs.
    uint64 blob_count;
    // only including data chunks
    uint64 blob_bytes;
    // including data chunks and parity chunks.
    uint64 blob_disk_bytes;
    // how many blob categories
    uint64 blob_category_count;
};

class BlobStor {
  public:
    virtual void SetDefaultTimeout(uint32 timeout_ms) = 0;

    virtual Status GetBlobStorAttr(BlobStorAttr* attr) = 0;

    // Category
    virtual Status AddCategory(const std::string& container_category) = 0;

    virtual Status DeleteCategory(const std::string& container_category) = 0;

    virtual Status GetCategoryAttr(const std::string& container_category,
                                   CategoryAttr* attr) = 0;

    virtual KVStorHolder* GetKVStorForCategory(
        const std::string& container_category, Status* status) = 0;

    virtual CategoryIterator* GetCategories() = 0;

    virtual Status GetCategoryCount(uint64* count) = 0;

    // Container
    virtual Status EncodeContainerKey(const std::string& container_category,
                                      const std::string& container_name,
                                      std::string* container_key) = 0;

    virtual Status DecodeContainerKey(const std::string& container_key,
                                      std::string* container_category,
                                      std::string* container_name) = 0;

    // Returns container_key even if container exists.
    virtual Status AddContainer(const std::string& container_category,
                                const std::string& container_name,
                                std::string* container_key) = 0;

    virtual Status DeleteContainer(const std::string& container_key) = 0;

    virtual Status GetContainerAttr(const std::string& container_key,
                                    ContainerAttr* attr) = 0;

    virtual KVStorHolder* GetKVStorForContainer(
        const std::string& container_key, Status* status) = 0;

    virtual ContainerIterator* GetContainers(
        const std::string& container_category) = 0;

    virtual BlobCategoryIterator* GetBlobCategories(
        const std::string& container_key, Status* status) = 0;

    // Blob
    virtual Status EncodeBlobKey(const std::string& blob_category,
                                 const std::string& blob_name,
                                 std::string* blob_key) = 0;

    virtual Status DecodeBlobKey(const std::string& blob_key,
                                 std::string* blob_category,
                                 std::string* blob_name) = 0;

    virtual AppendOnlyStreamBlob* AddStreamBlob(
        const std::string& container_key, const std::string& blob_category,
        const std::string& blob_name, const BlobWriteOptions& option,
        Status* status) = 0;

    virtual ReadOnlyStreamBlob* OpenStreamBlob(const std::string& container_key,
                                               const std::string& blob_key,
                                               Status* status) = 0;

    virtual AppendOnlyCompoundBlob* AddCompoundBlob(
        const std::string& container_key, const std::string& blob_category,
        const std::string& blob_name, const BlobWriteOptions& option,
        Status* status) = 0;

    virtual ReadOnlyCompoundBlob* OpenCompoundBlob(
        const std::string& container_key, const std::string& blob_key,
        Status* status) = 0;

    virtual Status DeleteBlob(const std::string& container_key,
                              const std::string& blob_key) = 0;

    virtual Status GetBlobAttr(const std::string& container_key,
                               const std::string& blob_key, BlobAttr* attr,
                               std::vector<BlobChunkAttr>* chunks = NULL) = 0;

    // return NULL if container_key format is invalid.
    virtual BlobIterator* GetBlobs(const std::string& container_key) = 0;

    virtual BlobIterator* GetBlobs(const std::string& container_key,
                                   const std::string& blob_category) = 0;
    virtual Status GetBlobCount(const std::string& container_key,
                                const std::string& blob_category,
                                uint64* count) = 0;

  protected:
    BlobStor() {
    }

    virtual ~BlobStor() {
    }

  private:

    DISALLOW_COPY_AND_ASSIGN(BlobStor);
};

} // namespace db

#endif /* BLOB_STOR_H_ */
