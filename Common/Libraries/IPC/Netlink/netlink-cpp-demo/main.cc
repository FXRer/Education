#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <linux/types.h>
#include <sys/socket.h>
#include <linux/rtnetlink.h>
#include "netlink_send.hh"
#include "netlink_listener.hh"
#include "netlink_event.hh"

using namespace std;

/**
 *
 *
 **/
void 
usage()
{
  fprintf(stdout, "-s   start with sending netlink request\n");
  fprintf(stdout, "-h   help message\n");
}

/**
 *
 *
 **/
int 
main(int argc, char* const argv[])
{
  int ch;
  bool send_request = false;
  bool debug = false;
  
  cout << "netlink_main()" << endl;
  
  while ((ch = getopt(argc, argv, "sdh")) != -1) {
    switch (ch) {
    case 's':
      send_request = true;
      break;
    case 'd':
      debug = true;
      break;
    case 'h':
      usage();
      exit(0);
    }
  }  
  
  NetlinkSend nl_send;
  NetlinkListener nl_listener;

  int sock = nl_listener.init();
  if (sock <= 0) {
    cerr << "test_netlink(), bad voodoo. exiting.." << endl;
    exit(1);
  }

  if (send_request) {
    cout << "sending initial netlink request" << endl;
    nl_listener.set_multipart(true);
    if (nl_send.send(sock, RTM_GETLINK) != 0) {
      cerr << "test_netlink(), error sending" << endl;
      exit(1);
    }
  }

  while (true) {
    //    cout << "test_netlink: now entering listening mode: " << endl;
    NetlinkEvent nl_event;
    if (nl_listener.process(nl_event) == true) {
      if (send_request) {
        if (nl_send.send(sock, RTM_GETADDR) != 0) {
          cerr << "test_netlink(), error sending" << endl;
          exit(1);
        }
        send_request = false;
      }
      else {
        nl_listener.set_multipart(false);
      }

      char buf[20];
      sprintf(buf, "%d", nl_event.get_index());
      cout << "results for " << nl_event.get_iface() << 
        "(" << string(buf) << ")" << endl;
      cout << "  running: " << 
        string(nl_event.get_running() ? "yes" : "no") << endl;
      cout << "  enabled: " << 
        string(nl_event.get_enabled() ? "yes" : "no") << endl;
      if (debug) {
        cout << "  ifinfomsg: " << nl_event.get_ifinfomsg() << endl;
      }

      if (nl_event.get_type() == RTM_DELLINK || 
          nl_event.get_type() == RTM_NEWLINK) {
        cout << "  type: " << 
          string(nl_event.get_type()==RTM_DELLINK?"DELLINK":"NEWLINK") << endl;
        cout << "  state: " << string(nl_event.is_link_up()?"UP":"DOWN") << endl;
        sprintf(buf, "%d", nl_event.get_mtu());
        cout << "  mtu: " << string(buf) << endl;
        cout << "  mac: " << nl_event.get_mac_str() << endl;
      }
      else if (nl_event.get_type() == RTM_DELADDR ||
        nl_event.get_type() == RTM_NEWADDR) {
        cout << "  type: " << 
          string(nl_event.get_type()==RTM_DELADDR?"DELADDR":"NEWADDR") << endl;
        cout << "  addr: " << nl_event.get_addr().str().c_str() << endl;
        cout << "  broadcast: " << nl_event.get_broadcast().str().c_str() << endl;
        char buf[20];
        sprintf(buf, "%d", nl_event.get_mask_len());
        cout << "  mask length: " << string(buf) << endl;
      }
      cout << endl;
    }
    else {
      //      cout << "didn't receive a message, sleeping for 1 second" << endl;
      sleep(1);
    }
  }
  exit(0);
}

