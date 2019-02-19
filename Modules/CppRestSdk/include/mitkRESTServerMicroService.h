#ifndef mitkRESTServerMicroService_h
#define mitkRESTServerMicroService_h

#include <QThread>
#include "cpprest/asyncrt_utils.h"
#include "cpprest/containerstream.h"
#include "cpprest/filestream.h"
#include "cpprest/http_listener.h"
#include "cpprest/json.h"
#include "cpprest/producerconsumerstream.h"
#include "cpprest/uri.h"

#include "MitkCppRestSdkExports.h"
#include <mitkIRESTManager.h>
#include <usGetModuleContext.h>
#include <usModule.h>
#include <usServiceTracker.h>
typedef web::http::experimental::listener::http_listener MitkListener;
typedef web::http::http_request MitkRequest;
typedef web::http::http_response MitkResponse;
typedef web::http::methods MitkRESTMethods;
typedef web::http::status_codes MitkRestStatusCodes;
typedef web::json::json_exception MitkJsonException;

namespace mitk
{
  class RESTServerMicroService : public QObject
  {
    Q_OBJECT 


  public:
    /**
     * @brief Creates an server listening to the given URI
     *
     * @param uri the URI at which the server is listening for requests
     */
    RESTServerMicroService(web::uri uri);
    ~RESTServerMicroService();

    web::uri GetUri();

  private:
    /**
     * @brief Handle for incoming GET requests
     *
     * @param MitkRequest incoming request object
     */
    void HandleGet(MitkRequest request);

    MitkListener m_Listener;
    web::uri m_Uri;

  public slots:
    /**
     * @brief Opens the listener and starts the listening process
     */
    void OpenListener();

    /**
     * @brief Closes the listener and stops the listening process
     */
    void CloseListener();
  };
} // namespace mitk
#endif