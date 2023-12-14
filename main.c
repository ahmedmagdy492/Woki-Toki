#include <gtk/gtk.h>

#include <string.h>

#ifdef _WIN32
#include "win_net_client.h"
#include "win_net_server.h"
#include <processthreadsapi.h>
#define WIN_HANDLE HANDLE
#define WIN_DWORD DWORD
#define WIN_LPVOID LPVOID
#elif __linux__
// TBD
#endif

#define WIDTH 768
#define HEIGHT 546

GtkWidget* window;
WIN_HANDLE server_thread_handle = NULL;

#ifdef _WIN32
WIN_DWORD server_bg_thread(WIN_LPVOID* data)
#else
WIN_DWORD server_bg_thread(void* data) // consider changing this to the linux one
#endif
{
  int is_running = 1, result = 0;

  result = init_server_socket();

  if(result != 0) {
    return -1;
  }

  result = setup_sock_server();
  if(result != 0) {
    cleanup();
    return -2;
  }

  char buffer[REC_BUFF_LEN];

  while(is_running) {
    result = accept_client();
    if(result == 0) {
      // start receiving data
      memset(buffer, 0, REC_BUFF_LEN);
      int len = 0;
      result = recv_data(buffer, &len);
      if(result == 0) {
	// successfully received data
	// TBD(ahmed): play the audio
      }
    }
  }

  cleanup();
  return 0;
}


// UI callbacks begin
static void sendBtnClicked(GtkWidget* btn, gpointer user_data) {
  GtkEntry* entry = (GtkEntry*)user_data;
  GtkEntryBuffer* ipEntryBuff = gtk_entry_get_buffer(entry);

  const char* ip = gtk_entry_buffer_get_text(ipEntryBuff);
  int ip_len = strlen(ip);

  if(strncmp(ip, "", ip_len) == 0) {
    GtkAlertDialog* dialog = gtk_alert_dialog_new("Please enter a valid IP");
    gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
  }
  else {
    int result = send_data("ahmed", 6, ip);

    if(result != 0) {
      GtkAlertDialog* dialog = gtk_alert_dialog_new("Connection result: %d", result);
      gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
    }
    else {
      GtkAlertDialog* dialog = gtk_alert_dialog_new("Sent Message");
      gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
    }
  }
}

static void on_active(GtkApplication* app, gpointer user_data) {
  window = gtk_application_window_new(app);

  gtk_window_set_title(GTK_WINDOW(window), "Woki Toki");
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);

  // add widgets
  GtkWidget* container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  
  GtkWidget* ip_txt = gtk_label_new("Please Enter IP of Other the Device");
  gtk_box_append(GTK_BOX(container), ip_txt);

  GtkWidget* entry = gtk_entry_new();
  gtk_box_append(GTK_BOX(container), entry);

  GtkWidget* sendBtn = gtk_button_new_with_label("Send");
  g_signal_connect(GTK_WIDGET(sendBtn), "clicked", G_CALLBACK(sendBtnClicked), entry);
  gtk_box_append(GTK_BOX(container), sendBtn);

  gtk_window_set_child(GTK_WINDOW(window), container);
  gtk_window_present(GTK_WINDOW(window));
}
// UI callbacks end


int main(int argc, char** argv) {

  // creating a new thread for the server
  #ifdef _WIN32
  server_thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)server_bg_thread, NULL, 0, NULL);
  #elif __linux__

  #endif

  GtkApplication* app = gtk_application_new("io.github.ahmedmagdy492", G_APPLICATION_DEFAULT_FLAGS);
  int status;
  
  g_signal_connect(app, "activate", G_CALLBACK(on_active), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  #ifdef _WIN32
  CloseHandle(server_thread_handle);
  #elif __linux__

  #endif

  return status;
}