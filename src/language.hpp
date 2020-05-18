#ifndef __LANG_HPP__
#define __LANG_HPP__

#include <string>

#define NUM_LANGUAGES 4
enum LangId { english, dutch, french, german };

struct Dictionary{
    std::string make_choice;
    std::string main_title;
    std::string main_text1;
    std::string main_text2;
    std::string lives;
    std::string score;
    std::string main_spaceproceed;
    std::string main_spacestart;
    std::string main_copyright;
    std::string submenuA_line1;
    std::string submenuA_line2;
    std::string submenuB_line1;
    std::string submenuB_line2;
    std::string submenuC_line1;
    std::string chooseDiff;
    std::string warpnextlevel_1;
    std::string warpnextlevel_2;
	std::string yourscore;
    std::string speed;
    std::string countdown;
};

class Lang
{
    private:

    public:
        Lang(LangId _langid = english) {
            setLanguage(_langid);
        };

        ~Lang() {};

        Dictionary dict;
        LangId current_lang;
        void cycle()
        {
            int current_language_index = current_lang;
            current_language_index++;
            if (current_language_index == NUM_LANGUAGES) current_language_index = 0;
            switch(current_language_index) {
                case 0:
                    current_lang = english;
                    break;    
                case 1:
                    current_lang = dutch;
                    break;    
                case 2:
                    current_lang = french;
                    break;    
                case 3:
                    current_lang = german;
                    break;    
            }
            setLanguage(current_lang);
        }


        void setLanguage(LangId langid)
        {
            current_lang = langid;
            switch (current_lang) {
                case english:

                    this->dict = {
                        "MAKE YOUR CHOICE",
                        "TIMESTABLES",
                        "learn the times-tables in a fun way",
                        "..with a computer and in 3D!!!",
                        "lives: ",
                        "score: ",
                        "PRESS SPACE TO PROCEED",
                        "PRESS SPACE TO START",
                        "Made by KG-soft MMXX",
                        "Successive Mode",
                        "Start from table:",
                        "Single Mode",
                        "Choose table:",
                        "Random Mode",
                        "Choose Difficulty:",
                        "WARP TO",
                        "NEXT LEVEL!!",
						"your score:",
                        "speed: ",
                        "countdown:"
                    };
                    break;
                case dutch:
                    this->dict = {
                        "MAAK JE KEUZE",
                        "TAFELTJES!!",
                        "leer de tafeltjes op een leuke manier",
                        "..met een computer en in 3D!!!",
                        "levens: ",
                        "score: ",
                        "DRUK OP SPATIE OM VERDER TE GAAN",
                        "DRUK OP SPATIE STARTEN",
                        "Gemaakt door KG-soft MMXX",
                        "Opeenvolgende Mode",
                        "Start met de tafel van:",
                        "Single Mode",
                        "Kies tafel van:",
                        "Willekeurige Mode",
                        "Kies Moeilijkheid:",
                        "OP NAAR",
                        "VOLGEND LEVEL!!",
						"jou score:",
                        "speed: ",
                        "countdown:"
                    };
                    break;
                case french:
                    this->dict = {
                        "FAITES VOTRE CHOIX",
                        "MULTIPLICATIONS",
                        "Apprendre la multiplication",
                        "..avec un ordinateur et en 3D!!",
                        "vives: ",
                        "score: ",
                        "APPUYEZ SUR ESPACE POUR CONTINUER",
                        "APPUYEZ SUR ESPACE POUR COMMENCER",
                        "fabrique par logiciel KG MMXX",
                        "Mode Successive",
                        "commencer par tableau:",
                        "Mode Solo",
                        "Choisissez table de:",
                        "Mode aleatoire",
                        "Difficulte:",
                        "PROCHAIN SORTIE:",
                        "LEVEL SUIVANT!!",
						"ton score:",
                        "speed: ",
                        "countdown:"
                    };
                    break;
                case german:
                    this->dict = {
                        "Treffen Sie Ihre Wahl",
                        "MULTIPLIZIEREN",
                        "lerne auf spielerische Weise zu multiplizieren",
                        "..mit einem computer und in 3D!!!",
                        "leben: ",
                        "score: ",
                        "DRUCKEN SIE DIE LEERTASTE UM FORTZUFAHREN",
                        "DRUCKEN SIE DIE LEERTASTE UM ZU STARTEN",
                        "hergestellt von KG-soft MMXX",
                        "Sukzessive Mode",
                        "Start mit der Tabelle von:",
                        "Single Mode",
                        "Wahlen Sie die Tabelle:",
                        "Zufallsmodus",
                        "Wahle die Schwierigkeit:",
                        "SCHNELL ZUM",
                        "NACHSTEN LEVEL!",
						"Ihre score:",
                        "speed: ",
                        "countdown:"
                    };
                    break;
            };
        };
};
#endif
