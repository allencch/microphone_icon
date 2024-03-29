#include <iostream>
#include <cstring>
#include "AlsaMic.hpp"

using namespace std;

// Channel
const int LEFT = 1;
const int RIGHT = 2;

bool controls_changed;

static const snd_mixer_selem_channel_id_t control_channels[][2] = {
  { SND_MIXER_SCHN_FRONT_LEFT, SND_MIXER_SCHN_FRONT_RIGHT },
  { SND_MIXER_SCHN_REAR_LEFT, SND_MIXER_SCHN_REAR_RIGHT },
  { SND_MIXER_SCHN_FRONT_CENTER, SND_MIXER_SCHN_UNKNOWN },
  { SND_MIXER_SCHN_WOOFER, SND_MIXER_SCHN_UNKNOWN },
  { SND_MIXER_SCHN_SIDE_LEFT, SND_MIXER_SCHN_SIDE_RIGHT },
};

snd_mixer_t* AlsaMic::get_mixer() {
  return mixer;
}

char* AlsaMic::get_mixer_device_name() {
  return mixer_device_name;
}

snd_mixer_selem_id_t* AlsaMic::get_selem_id() {
  return current_selem_id;
}

int AlsaMic::elem_callback(snd_mixer_elem_t *elem, unsigned int mask) {
  if (mask & (SND_CTL_EVENT_MASK_REMOVE |
              SND_CTL_EVENT_MASK_INFO |
              SND_CTL_EVENT_MASK_VALUE)) {
    controls_changed = true;
  }
  return 0;
}

int AlsaMic::mixer_callback(snd_mixer_t *mixer, unsigned int mask, snd_mixer_elem_t *elem) {
  if (mask & SND_CTL_EVENT_MASK_ADD) {
    snd_mixer_elem_set_callback(elem, AlsaMic::elem_callback);
    controls_changed = true;
  }
  return 0;
}

void AlsaMic::create_mixer_object() {
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

  // snd_mixer_set_callback(mixer, AlsaMic::mixer_callback);
  err = snd_mixer_load(mixer);
  if (err < 0) {
    cout << "cannot load mixer controls" << endl;
  }

  err = snd_mixer_selem_id_malloc(&current_selem_id); // TODO: Free
  if (err < 0) {
    cout << "out of memory" << endl;
  }
}

void AlsaMic::mixer_shutdown() {
  if (mixer) {
    snd_mixer_close(mixer);
    mixer = NULL;
  }
  if (current_selem_id) {
    snd_mixer_selem_id_free(current_selem_id);
    current_selem_id = NULL;
  }
}

void AlsaMic::reload() {
  micStatusMutex.lock();
  snd_mixer_free(mixer);
  snd_mixer_load(mixer);
  char name[] = "Capture";
  elem = get_mixer_elem_by_name(mixer, name);
  micStatusMutex.unlock();
}

snd_mixer_elem_t* AlsaMic::get_mixer_elem_by_name(snd_mixer_t* mixer, char* name) {
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


bool AlsaMic::get_channels(snd_mixer_elem_t* elem) {
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
  return has_switch;
}

bool AlsaMic::is_elem_capturing(snd_mixer_elem_t* elem, bool* has_switch) {
  *has_switch = get_channels(elem);
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

bool AlsaMic::toggle_switches(snd_mixer_elem_t* elem,
                     snd_mixer_selem_channel_id_t* cswitch_channels,
                     unsigned int channels,
                     bool has_switch) {
  snd_mixer_selem_channel_id_t channel_ids[2];
  int left;
  int right;
  int err;

  channel_ids[0] = cswitch_channels[0];
  channel_ids[1] = cswitch_channels[1];
  if (has_switch) {
    channels = LEFT;
  }
  if (channels & LEFT) {
    err = snd_mixer_selem_get_capture_switch(elem, channel_ids[0], &left);
    if (err < 0)
      return left;
  }
  if (channels & RIGHT) {
    err = snd_mixer_selem_get_capture_switch(elem, channel_ids[1], &right);
    if (err < 0)
      return right;
  }
  if (channels & LEFT) {
    snd_mixer_selem_set_capture_switch(elem, channel_ids[0], !left);
  }
  if (channels & RIGHT) {
    snd_mixer_selem_set_capture_switch(elem, channel_ids[1], !right);
  }
  return !left;
}


AlsaMic::AlsaMic() {
  selem_regopt = { .ver = 1,
                   .abstract = SND_MIXER_SABSTRACT_NONE,
                   .device = "default", };
  create_mixer_object();
  mixer = get_mixer();
  char name[] = "Capture";
  elem = get_mixer_elem_by_name(mixer, name);
}

AlsaMic::~AlsaMic() {
  mixer_shutdown();
}

bool AlsaMic::isCapturing() {
  micStatusMutex.lock();
  auto result = is_elem_capturing(elem, &has_switch);
  micStatusMutex.unlock();
  return result;
}

bool AlsaMic::toggle() {
  micStatusMutex.lock();
  auto result = toggle_switches(elem, cswitch_channels, LEFT | RIGHT, has_switch);
  micStatusMutex.unlock();
  return result;
}
