#ifndef MICROPHONE_ICON_HPP
#define MICROPHONE_ICON_HPP

#include <string>
#include <gtk/gtk.h>
#include "AlsaMic.hpp"

using namespace std;

class MicrophoneIcon {
public:
  explicit MicrophoneIcon();
  ~MicrophoneIcon();

  int run(int argc, char** argv);

  static void activate(GtkApplication* app, gpointer userData);
  static void activateStatusIcon(GtkStatusIcon* icon, gpointer userData);
  static void rightClickStatusIcon(GtkStatusIcon* icon, guint button, guint activationTime, gpointer userData);
  static void quit(GtkMenuItem* menuItem, gpointer userData);
  static gboolean detectCapturing(gpointer userData);

  AlsaMic* getMic();
  bool getCapturingStatus();
  void setCapturingStatus(bool capturing);
  void updateStatusIcon();

private:
  void setupMic();
  void setupStatusIcon();
  void setupIconMenu();
  GtkApplication* app;
  GtkStatusIcon* icon;
  GtkWidget* menu;

  string getIcon(bool on);

  AlsaMic* alsaMic;
  bool isCapturing;
};

#endif
