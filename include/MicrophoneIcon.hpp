#include <gtk/gtk.h>

class MicrophoneIcon {
public:
  explicit MicrophoneIcon();

  int run(int argc, char** argv);

  static void activate(GtkApplication* app, gpointer userData);
  static void activateStatusIcon(GtkStatusIcon* icon, gpointer userData);

private:
  void setupStatusIcon();
  GtkStatusIcon* icon;
};
