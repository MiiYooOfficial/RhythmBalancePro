# RhythmBalancePro
*A music theory homework solver for Windows*

RhythmBalancePro is a terminal-based, Windows application, that solves what we call "Rest Insertion Problems," a common type of question found in rudimentary music theory. The application can either be used as a standalone, or in conjunction with both Audiveris (an OMR engine) and Musescore (a music notation program).

### RhythmBalancePro as a standalone
To use RhythmBalancePro as a standalone application, problems MUST be entered as a text file and answers will correspondingly also be returned as a text file. However, given that the application is meant to solve music theory questions, the user will first have to translate the problem from music notation to an ASCII-based notation system unique to the program (see Instruction Manual under Releases to learn more).

### RhythmBalancePro in conjunction with Audiveris and Musescore
The alternative to having to translate every question into the program's own ASCII-based notation system is to use the application in combination with Audiveris, an optical music recognition (OMR) engine and Musescore, a music notation software. Audiveris will be used to digitially scan and interpret the homework directly from an image or a PDF scan of the questions. Musescore will then be used to make minor tweaks and edits before the user can feed the resulting file into RhythmBalancePro. The solutions, outputted as a MusicXML file, can then be opened via Musescore to reveal the answers in music notation format, thus allowing the user to essentially avoid any sort of notation other than music notation. The reason why Audiveris is used to scan the original problems as opposed to having the user directly compose every question in Musescore is because Musescore does not tolerate incomplete measures, thus making the task of entering problems virtually impossible. 

To learn more about the program and how to use it, check out the program's latest release.
