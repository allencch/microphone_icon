#include <gtk/gtk.h>
#include <alsa/asoundlib.h>

class MicrophoneIcon {
public:
  explicit MicrophoneIcon();
  ~MicrophoneIcon();

  int run(int argc, char** argv);

  static void activate(GtkApplication* app, gpointer userData);
  static void activateStatusIcon(GtkStatusIcon* icon, gpointer userData);

private:
  void setupMic();
  void setupStatusIcon();
  GtkStatusIcon* icon;

  snd_mixer_t* mixer;
};
