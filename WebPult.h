class nifServer {
  private:
    unsigned long pultCommands[24];

  public:
    String clientRequest;
    nifServer() {

      init();
    }
    void init() {
      wifiSetup();
      //pinMode(pin, OUTPUT);
      //off();
    }
    void serverLoop() {
      checkClient();
      
    }
    bool checkClient() {

    }

    String getClientRequest() {
      String ClientRequest;
      ClientRequest = "";

      return ClientRequest;
    }

    void newRoute(String title, String address, String &html, bool gStyle ) {

    }

};
