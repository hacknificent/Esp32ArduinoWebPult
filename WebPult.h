  class WebPult {
    private:
      unsigned long pultCommands[24] {
        0xFF00FF, 0xFF40BF, 0xFF609F, 0xFFE01F,
        0xFF10EF, 0xFF906F, 0xFF50AF, 0xFFC03F,
        0xFF30CF, 0xFFB04F, 0xFF708F, 0xFFF00F,
        0xFF08F7, 0xFF8877, 0xFF48B7, 0xFFC837,
        0xFF28D7, 0xFFA857, 0xFF6897, 0xFFE817,
        0xFF18E7, 0xFF9867, 0xFF58A7, 0xFFD827
      };
      String pultButtons[24][3] = {
        {"+", "", ""}, // 0
        {"-", "", ""}, // 1
        {"off", "000", ""}, // 2
        {"on", "e80101", ""}, // 3

        {"R", "e80101", ""}, // 4
        {"G", "02cc02", ""}, // 5
        {"B", "403FFD", ""}, // 6
        {"W", "", ""}, // 7

        {"", "ff6000", ""}, // 8
        {"", "46ffc5", ""}, // 9
        {"", "605df7", ""}, // 10
        {"Flash", "", ""}, // 11

        {"", "ffa500", ""}, //12
        {"", "5afff8", ""}, // 13
        {"", "ffc4fd", ""}, // 14
        {"Strobe", "", ""}, // 15

        {"", "ffd125", ""}, // 16
        {"", "4fd3ff", ""}, // 17
        {"", "f5a0ff", ""}, // 18
        {"Smooth", "", ""}, // 19

        {"", "ffff00", ""}, // 20
        {"", "6b9eff", ""}, // 21
        {"", "ff92ce", ""}, // 22
        {"Mode", "", ""}, // 23

      };
      
    public:
      String pultPageLayout;
      WebPult() {
        // Use 'this->' to make the difference between the
        // 'pin' attribute of the class and the
        // local variable 'pin' created from the parameter.
        //this->pin = pin;
        init();
      }
      void init() {
        setPultLayout();
        //pinMode(pin, OUTPUT);
        //off();
      }
      void setPultLayout() {

        char *pultBtns[24];
        for (int i = 0; i < 24; i++) {
          pultBtns[i] = "<li class=\"pultButton\"";

          if (this->pultButtons[i][1] && this->pultButtons[i][1] != "") {
            pultBtns[i] = pultBtns[i] , "style = \"color: #" , this->pultButtons[i][1] , ";\""; // Set button color
          }
          pultBtns[i] = pultBtns[i] , ">";

          if (this->pultButtons[i][0] && this->pultButtons[i][0] != "") {
            pultBtns[i] = pultBtns[i] , "<span>", this->pultButtons[i][0] , "</span>"; // Set button Label
          }

          pultBtns[i] = pultBtns[i], "<input type=\"submit\" name=\"pBtn\" value=\"" + String(i) + "c\">";
          pultBtns[i] = pultBtns[i], "</li>";
        }


        this->pultPageLayout =
          "<form action=\"pult\" target=\"_self\" method=\"get\" >",
          "<ul class=\"pult\">",
          pultBtns,
          "</ul></form>";

      }

      String getPultLayout() {
        return this->pultPageLayout;

      }

      void setPultCommands(unsigned long pultCommands[24]) {
        for (int i = 0; i < 24; i++) {
          this->pultCommands[i] = pultCommands[i];
        }
      }


  };
//WebPult nifWebPult;
