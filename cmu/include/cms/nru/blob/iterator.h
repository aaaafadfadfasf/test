#ifndef ITERATOR_H_
#define ITERATOR_H_

#include "misc.h"
#include "status.h"

namespace cdb {

struct CategoryAttr;
struct ContainerAttr;
struct BlobAttr;

class Iterator {
  public:
    Iterator() {
    }

    virtual ~Iterator() {
    }

    virtual Status Init() = 0;
    virtual Status Next() = 0;

    // Current item is valid.
    virtual bool Valid() = 0;

    // container_category, container_key , blob_key, key(k/v), attachment name,  item name
    virtual const std::string& key() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(Iterator);
};

class AttachmentIterator : public Iterator {
  public:
    AttachmentIterator() {
    }

    virtual ~AttachmentIterator() {
    }

    virtual uint32 size() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(AttachmentIterator);
};

// CAUTION: not thread-safe.
class CategoryIterator : public Iterator {
  public:
    CategoryIterator() {
    }
    virtual ~CategoryIterator() {
    }

    virtual const CategoryAttr& attr() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(CategoryIterator);
};

// CAUTION: not thread-safe.
class ContainerIterator : public Iterator {
  public:
    ContainerIterator() {
    }
    virtual ~ContainerIterator() {
    }

    virtual const ContainerAttr& attr() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(ContainerIterator);
};

class BlobCategoryIterator : public Iterator {
  public:
    BlobCategoryIterator() {
    }
    virtual ~BlobCategoryIterator() {
    }
    // how many blobs within this blob category
    virtual uint64 blob_count() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(BlobCategoryIterator);
};

// CAUTION: not thread-safe.
class BlobIterator : public Iterator {
  public:
    BlobIterator() {
    }

    virtual ~BlobIterator() {
    }

    virtual const BlobAttr& attr() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(BlobIterator);
};

class ItemIterator : public Iterator {
  public:
    ItemIterator() {
    }

    virtual ~ItemIterator() {
    }

    virtual Status Init() = 0;
    virtual Status Next() = 0;

    // Current item is valid.
    virtual bool Valid() = 0;

    virtual uint32 item_id() = 0;

  private:
    DISALLOW_COPY_AND_ASSIGN(ItemIterator);
};

} // namespace cdb

#endif /* ITERATOR_H_ */
