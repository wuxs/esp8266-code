
class Utils {

public:

    char wake_mac[48];
    char wifi_ssid[64];
    char wifi_password[64];
    char notify_host[100];
    int  notify_port = 80;
    char notify_url[];


    void init_wifi();

    void init_time();

    void init_mqtt();

    void now_time();

    bool wake_on_lan();

private:

    bool make_magic_packet();


};



