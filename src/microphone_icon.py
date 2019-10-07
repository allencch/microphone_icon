# StatusIcon is deprecated. But I need it.
# WTF it is removed?

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
from pyalsa import alsamixer
import inspect


class MicrophoneIcon():
    MUTE = 'MUTE'
    UNMUTE = 'UNMUTE'

    def __init__(self):
        self.status = MicrophoneIcon.UNMUTE
        self.load_mixer()

    def load_mixer(self):
        mixer = alsamixer.Mixer()
        mixer.attach()
        mixer.load()
        elem = alsamixer.Element(mixer, 'Capture')
        print(elem)
        print(alsamixer.channel_name)
        members = inspect.getmembers(elem)
        for m in members:
            print(m)
        print(elem.has_switch(True))
        print(elem.get_switch_tuple(True))
        print(elem.get_volume(0))
        self.mixer = mixer

    def setup_icon(self):
        self.icon = Gtk.StatusIcon.new_from_file('images/microphone-on.png')
        self.icon.connect('activate', self.toggle)

    def toggle(self, status_icon):
        if self.status == MicrophoneIcon.UNMUTE:
            self.status = MicrophoneIcon.MUTE
        else:
            self.status = MicrophoneIcon.UNMUTE
        print(self.status)

    def call(self):
        # self.win = Gtk.Window()
        # self.win.connect('destroy', Gtk.main_quit)
        # self.win.show_all()

        self.setup_icon()

        # Gtk.main()


if __name__ == '__main__':
    MicrophoneIcon().call()
