#ifndef MICROPHONE_ICON_HPP
#define MICROPHONE_ICON_HPP

#include <gtk/gtk.h>
#include "AlsaMic.hpp"

class MicrophoneIcon {
public:
  explicit MicrophoneIcon();
  ~MicrophoneIcon();

  int run(int argc, char** argv);

  static void activate(GtkApplication* app, gpointer userData);
  static void activateStatusIcon(GtkStatusIcon* icon, gpointer userData);

  AlsaMic* getMic();

private:
  void setupMic();
  void setupStatusIcon();
  GtkStatusIcon* icon;

  AlsaMic* alsaMic;
};

#endif
