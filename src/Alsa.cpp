#include <iostream>
#include <cstring>
#include "Alsa.hpp"

using namespace std;

snd_mixer_t *mixer;
char *mixer_device_name;
snd_mixer_selem_id_t *current_selem_id;
snd_mixer_selem_channel_id_t cswitch_channels[2];

static const snd_mixer_selem_channel_id_t control_channels[][2] = {
  { SND_MIXER_SCHN_FRONT_LEFT, SND_MIXER_SCHN_FRONT_RIGHT },
  { SND_MIXER_SCHN_REAR_LEFT, SND_MIXER_SCHN_REAR_RIGHT },
  { SND_MIXER_SCHN_FRONT_CENTER, SND_MIXER_SCHN_UNKNOWN },
  { SND_MIXER_SCHN_WOOFER, SND_MIXER_SCHN_UNKNOWN },
  { SND_MIXER_SCHN_SIDE_LEFT, SND_MIXER_SCHN_SIDE_RIGHT },
};

snd_mixer_selem_regopt selem_regopt = {
  .ver = 1,
  .abstract = SND_MIXER_SABSTRACT_NONE,
  .device = "default",
};

snd_mixer_t* get_mixer() {
  return mixer;
}

char* get_mixer_device_name() {
  return mixer_device_name;
}

snd_mixer_selem_id_t* get_selem_id() {
  return current_selem_id;
}

void create_mixer_object() {
    int err;

    err = snd_mixer_open(&mixer, 0); // TODO: Close
    if (err < 0) {
      cout << "cannot open mixer" << endl;
    }

    mixer_device_name = strdup(selem_regopt.device);
    err = snd_mixer_selem_register(mixer, &selem_regopt, NULL);
    if (err < 0) {
      cout << "cannot open mixer" << endl;
    }

    // snd_mixer_set_callback(mixer, mixer_callback);

    err = snd_mixer_load(mixer);
    if (err < 0) {
      cout << "cannot load mixer controls" << endl;
    }

    err = snd_mixer_selem_id_malloc(&current_selem_id); // TODO: Free
    if (err < 0) {
      cout << "out of memory" << endl;
    }
}

void mixer_shutdown() {
  if (mixer) {
    snd_mixer_close(mixer);
  }
  if (current_selem_id) {
    snd_mixer_selem_id_free(current_selem_id);
  }
}

snd_mixer_elem_t* get_mixer_elem_by_name(snd_mixer_t* mixer, char* name) {
  auto elem = snd_mixer_first_elem(mixer);
  if (strcmp(snd_mixer_selem_get_name(elem), name) == 0) {
    return elem;
  }
  elem = snd_mixer_elem_next(elem);
  while (elem) {
    if (strcmp(snd_mixer_selem_get_name(elem), name) == 0) {
      return elem;
    }
    elem = snd_mixer_elem_next(elem);
  }
  return NULL;
}


void allocate_channels(snd_mixer_elem_t* elem) {
  bool has_switch = false;
  if (snd_mixer_selem_has_capture_switch_joined(elem)) {
    has_switch = false;
    cswitch_channels[0] = (snd_mixer_selem_channel_id_t)0;
  } else {
    if (snd_mixer_selem_has_capture_channel(elem, control_channels[0][0])) {
      has_switch = false;
      cswitch_channels[0] = control_channels[0][0];
    }
    if (control_channels[0][1] != SND_MIXER_SCHN_UNKNOWN &&
        snd_mixer_selem_has_capture_channel(elem, control_channels[0][1])) {
      has_switch = true;
      cswitch_channels[1] = control_channels[0][1];
    }
  }
  if (has_switch) {
    cswitch_channels[0] = cswitch_channels[1];
  }
}

bool is_elem_capturing(snd_mixer_elem_t* elem) {
  allocate_channels(elem);
  int sw;
  int err = snd_mixer_selem_get_capture_switch(elem, cswitch_channels[0], &sw);
  if (err >= 0) {
    err = snd_mixer_selem_get_playback_switch(elem, cswitch_channels[1], &sw);
  }
  if (err >= 0 && !sw) {
    cout << "is_capturing error" << endl;
  }
  return (bool)sw;
}


AlsaMic::AlsaMic() {
  create_mixer_object();
  mixer = get_mixer();
  char name[] = "Capture";
  elem = get_mixer_elem_by_name(mixer, name);
}

AlsaMic::~AlsaMic() {
  if (mixer) {
    snd_mixer_free(mixer);
    snd_mixer_close(mixer);
  }
  mixer_shutdown();
}

bool AlsaMic::isCapturing() {
  return is_elem_capturing(elem);
}
