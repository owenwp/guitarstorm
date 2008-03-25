Guitar Storm Prototype Version
Copyright 2008 Zombie Process

http://guitarstormgame.com
http://zombieprocess.org

To run this program, you must first install an ASIO compatable driver, if you do not have one already.  Consult the specifications of your sound device.  If you do not have one, you may use ASIO4ALL, which is included in this distribution.  Once installed, reduce the buffer size/latency as far as it will go without creating popping and skipping during gameplay.  If you still cannot eliminate the popping, increase the ingame buffer size by using the "-sample <size>" command argument, with a size such as 256, 512, or 1024.  Then repeat adjustment of the ASIO settings.  Larger sizes will result in increased playback latency.

During play, use the arrow keys and enter to select a song, then play along.  Press the 1-9 keys to reduce the speed of the song to 10-90%, and Backspace to return to 100%.  Press 0 to have it stop at each note, so you can play at your own pace and familiarize yourself with the fingering of the song.  Once the song ends, press enter to return to the main menu.  You can also press F to toggle fullscreen mode, and ESC to quit.

To import your own songs, you can download any Guitar Pro format tab from the web, or make your own, and place it in the tabs/ folder.  Many tabs will not load correctly, so it is recommended that you use a program like TuxGuitar to open and re-save the tab before playing.  It is also necessary to set whichever guitar track you want to play to track position 1, by moving it to the top of the track list.  Also keep in mind that not all note markups are supported, so you won't see any notations for sustains, bends, slides, ho/po, etc.

Source Code is freely available at http://guitarstorm.googlecode.com