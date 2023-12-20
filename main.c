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
#include "lin_net_client.h"
#include "lin_net_server.h"
#include <pthread.h>
#include <errno.h>
#endif

#define WIDTH 600
#define HEIGHT 400

static GtkWidget* window;
static int recording = 0;

void show_dialog(char* text) {
  #ifdef _WIN32
  GtkAlertDialog* dialog = gtk_alert_dialog_new(text);
  gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
  #endif
  // TODO: find another option for gtk versions before 4.10
}

#ifdef _WIN32
WIN_HANDLE server_thread_handle = NULL;
WIN_DWORD server_bg_thread(WIN_LPVOID* data)
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
    printf("Someone has just connected\n");
    if(result == 0) {
      // start receiving data
      memset(buffer, 0, REC_BUFF_LEN);
      int len = 0;
      result = recv_data(buffer, &len);
      if(result == 0) {
	
      }
    }
  }

  cleanup();
  return 0;
}
#elif __linux__
void* server_bg_thread(void* data)
{
  int is_running = 1, result = 0;

  result = setup_sock_server();
  if(result != 0) {
    cleanup();
    return NULL;
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
  return NULL;
}
#endif


// UI callbacks begin
static void sendBtnClicked(GtkWidget* btn, gpointer user_data) {
  GtkEntry* entry = (GtkEntry*)user_data;
  GtkEntryBuffer* ipEntryBuff = gtk_entry_get_buffer(entry);

  const char* ip = gtk_entry_buffer_get_text(ipEntryBuff);
  int ip_len = strlen(ip);

  if(strncmp(ip, "", ip_len) == 0) {
    show_dialog("Please enter a valid IP");
  }
  else {
    int result;

    // TODO: find some type of simple audio library to use

    if(1) {

    }
    else {
      result = send_data("failed to record", 6, ip);

      if(result != 0) {
	switch(result) {
	case -1:
	  show_dialog("Network Error");
	  break;
	case -2:
	  show_dialog("Network Error");
	  break;
	case -3:
	  show_dialog("Please enter a valid IP");
	case -4:
	  show_dialog("Unable to connect to remote server");
	  break;
	}
      }
      else {
	show_dialog("Sent Message");
      }
    }
  }
}

static void on_active(GtkApplication* app, gpointer user_data) {
  window = gtk_application_window_new(app);

  gtk_window_set_title(GTK_WINDOW(window), "Woki Toki");
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  // add widgets
  GtkWidget* container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_top(GTK_WIDGET(container), 100);
  
  GtkWidget* ip_txt = gtk_label_new("Please enter the IP of the other device");
  gtk_box_append(GTK_BOX(container), ip_txt);

  GtkWidget* entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "IP Address i.e 0.0.0.0");
  gtk_widget_set_size_request(GTK_WIDGET(entry), 100, -1);
  gtk_widget_set_hexpand(GTK_WIDGET(entry), FALSE);
  gtk_widget_set_halign(GTK_WIDGET(entry), GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(container), entry);

  GtkWidget* sendBtn = gtk_button_new_with_label("Record and Send");
  gtk_widget_set_size_request(GTK_WIDGET(sendBtn), 100, -1);
  gtk_widget_set_hexpand(GTK_WIDGET(sendBtn), FALSE);
  gtk_widget_set_halign(GTK_WIDGET(sendBtn), GTK_ALIGN_CENTER);
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
  pthread_t thread_id;
  int result = 0;
  result = pthread_create(&thread_id, NULL, server_bg_thread, NULL);
  if(result == -1) {
    perror("while create background thread");
  }
  #endif

  GtkApplication* app = gtk_application_new("io.github.ahmedmagdy492", G_APPLICATION_DEFAULT_FLAGS);
  int status;
  
  g_signal_connect(app, "activate", G_CALLBACK(on_active), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  printf("Cleaning up...\n");

  #ifdef _WIN32
  CloseHandle(server_thread_handle);
  #elif __linux__
  pthread_cancel(thread_id);
  #endif

  return status;
}
