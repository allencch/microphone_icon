#include <alsa/asoundlib.h>

snd_mixer_t* get_mixer();
char* get_mixer_device_name();
snd_mixer_selem_id_t* get_selem_id();
snd_mixer_elem_t* get_mixer_elem();

void create_mixer_object(struct snd_mixer_selem_regopt* selem_regopt);
void mixer_shutdown();

snd_mixer_elem_t* get_mixer_elem_by_name(snd_mixer_t* mixer, char* name);
void allocate_channels(snd_mixer_elem_t* elem);

bool is_capturing(snd_mixer_elem_t* elem);
