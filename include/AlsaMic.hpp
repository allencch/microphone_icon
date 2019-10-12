#ifndef ALSA_MIC_HPP
#define ALSA_MIC_HPP

#include <alsa/asoundlib.h>

class AlsaMic {
public:
  AlsaMic();
  ~AlsaMic();
  bool isCapturing();
  void toggle();

  snd_mixer_t* get_mixer();
  char* get_mixer_device_name();
  snd_mixer_selem_id_t* get_selem_id();
  void reload();

  static int elem_callback(snd_mixer_elem_t *elem, unsigned int mask);
  static int mixer_callback(snd_mixer_t *mixer, unsigned int mask, snd_mixer_elem_t *elem);

private:
  void create_mixer_object();
  void mixer_shutdown();
  snd_mixer_elem_t* get_mixer_elem_by_name(snd_mixer_t* mixer, char* name);
  bool get_channels(snd_mixer_elem_t* elem);
  bool is_elem_capturing(snd_mixer_elem_t* elem, bool* has_switch);
  void toggle_switches(snd_mixer_elem_t* elem,
                       snd_mixer_selem_channel_id_t* cswitch_channels,
                       unsigned int channels,
                       bool has_switch);

  snd_mixer_t* mixer;
  snd_mixer_elem_t* elem;
  bool has_switch;
  snd_mixer_selem_regopt selem_regopt;

  char *mixer_device_name;
  snd_mixer_selem_id_t *current_selem_id;
  snd_mixer_selem_channel_id_t cswitch_channels[2];
};

#endif
