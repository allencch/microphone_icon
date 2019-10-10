#ifndef ALSA_MIC_HPP
#define ALSA_MIC_HPP

#include <alsa/asoundlib.h>

// TODO: Refactor to object
snd_mixer_t* get_mixer();
char* get_mixer_device_name();
snd_mixer_selem_id_t* get_selem_id();
snd_mixer_elem_t* get_mixer_elem();

void create_mixer_object();
void mixer_shutdown();
void toggle_switches(unsigned int type, unsigned int channels);

snd_mixer_elem_t* get_mixer_elem_by_name(snd_mixer_t* mixer, char* name);
bool allocate_channels(snd_mixer_elem_t* elem);

bool is_elem_capturing(snd_mixer_elem_t* elem, bool* has_switch);


class AlsaMic {
public:
  AlsaMic();
  ~AlsaMic();
  bool isCapturing();
  void toggle();

private:
  snd_mixer_t* mixer;
  snd_mixer_elem_t* elem;
  bool has_switch;
};

#endif
