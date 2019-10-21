#ifndef CTable_CLIENT_H_
#define CTable_CLIENT_H_

#include "kv_stor.h"
#include "blob_stor.h"

// cdb stands for "Cloud database".
namespace cdb {

static const std::string cdb_header_file_version = "0.2";

class CdbClient {
  public:
    virtual ~CdbClient() {
    }

    virtual KVStor* kv_stor() = 0;

    virtual BlobStor* blob_stor() = 0;

  protected:
    CdbClient() {
    }

  private:
    DISALLOW_COPY_AND_ASSIGN(CdbClient);
};

struct ServerPort {
    std::string primary_server_ip;
    // second ip can be empty.
    std::string second_server_ip;
    uint32 port;
};

CdbClient* NewCdbClient(const std::vector<ServerPort>& cluster,
                        const std::string& zone, const std::string& machine_id,
                        uint32 timeout_secs, const std::string& user,
                        const std::string& password, Status* status,
                        std::string cdb_header_file_version =
                            cdb_header_file_version);

const std::string& GetCdbVersion();

} // namespace

#endif /* CDB_CLIENT_H_ */
