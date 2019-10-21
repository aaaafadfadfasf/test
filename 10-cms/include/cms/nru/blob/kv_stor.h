#ifndef KV_STOR_H_
#define KV_STOR_H_

#include "misc.h"
#include "status.h"

namespace cdb {

class KVStor {
  public:
    class Transaction {
      public:
        virtual ~Transaction() {
        }

        // finalized has effect only when KVStor is for blob.
        // If finalized = true, key can't be put and deleted anymore. It only can be deleted when blob is deleted.
        // key can't start with '$'.
        virtual void Put(const std::string& key, const std::string& value,
                         bool finalized = false) = 0;
        virtual void Delete(const std::string& key) = 0;

        virtual Status Commit() = 0;

      protected:
        Transaction() {
        }

      private:
        DISALLOW_COPY_AND_ASSIGN(Transaction);
    };

    virtual Status Get(const std::string& key, std::string* value) = 0;

    // finalized has effect only when KVStor is for blob.
    // If finalized = true, key can't be put and deleted anymore. It only can be deleted when blob is deleted.
    virtual Status Put(const std::string& key, const std::string& value,
                       bool finalized = false) = 0;
    virtual Status Delete(const std::string& key) = 0;

    virtual Transaction* NewTransaction() = 0;

  protected:
    KVStor() {
    }

    virtual ~KVStor() {
    }

  private:
    DISALLOW_COPY_AND_ASSIGN(KVStor);
};

class KVStorHolder {
  public:
    virtual ~KVStorHolder() {
    }
    virtual KVStor* kv_stor() = 0;

  protected:
    KVStorHolder() {
    }

  private:

    DISALLOW_COPY_AND_ASSIGN(KVStorHolder);
};

} // namespace db

#endif /* KV_STOR_H_ */
