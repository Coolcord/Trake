Trake
=====

Competitive Tron and Snake written in C++11 with Allegro 5


-===============================- About Trake -===============================-

 Trake started out as a small learning project to allow me to become more
 familiar with the Allegro 5 libraries. In order to learn Allegro 5, I figured
 that I would start by making a simple game like snake. However, I ended up
 having way too many ideas for the game, and it evolved into what it is now.

 Trake is a competitive version of snake and the light cycles mode from Tron.
 The name is a merge of the words Tron and Snake. The game supports up to 4
 simultaneous players who must gather pellets and try to stay alive as long as
 possible.

 I would love to keep working on Trake, but unfortunately, I have other projects
 planned that are higher priority. However, in the game's current state, I'd
 consider it stable and complete. I have many, many more ideas for it, but I
 simply don't have the time to keep working on it. It is for this reason that
 I have open sourced the game. If anyone has any interesting features that
 they would like to add or contribute, they are free to do so.

 It is also important to note that the music was not created by me. It is by
 Daft Punk from their Tron albums. All copyrights to those songs belong to them.
 Keep this in mind if you intend to make use of the source code of this project.

 Trake is licensed under the MIT License. All files contained in this project,
 excluding the ones in the Music folders, are made by me and are covered by the MIT
 License. Anyone is free to make use of Trake's source code for anything they wish,
 but they must give me credit. For more information, read the included LICENSE file.
 
 If you wish to contact me about the game, or anything of the like,
 feel free to send me an email at coolcord24@gmail.com



-==========================- Gameplay Information -==========================-

 Trake has two gameplay modes: Snake and Tron. In Snake, the players must collect
 pellets to make their snake larger. If a snake collides with another player or
 itself, it dies and is out of the round. Dead snakes are still considered obstacles,
 so colliding with them will kill the player as well.

 In Tron, the players' snakes are constantly growing. Death is inevitable, so the goal
 is to stay alive for as long as possible. There are no pellets in Tron.


 Trake has two win conditions: Score and Survival. Score works differently depending
 on the gameplay mode. In Snake, points are awarded to players every time they eat
 a pellet. In Tron, one point is awarded to each player for every safe move they make.
 It is important to note that when the Score win condition is in play, the round will
 not end until every player is dead.

 In Survival, every time a player dies, every current living player gains 5 points.
 This win condition works the same in Snake and Tron. The round ends when there is only
 one player standing.


 Trake also has a rounds system. This will repeat the same gameplay mode and win condition
 the specified number of times. All points will carry over between rounds, and the
 winner will be declared after the final round.


 Trake can be played with 1-4 players. Trake also has AI players that can take the place
 of any human players. In both modes, the longer the round lasts, the faster the game will
 go, which will in turn increase the difficulty. 



-===============================- Under the Hood -===============================-

 Part of the reason why I chose to make snake as my first game under Allegro was to
 try to make use of an interesting technique in frame drawing. Normally, most
 modern games redraw the entire frame every time before it is sent off to the monitor.
 This is normally completely necessary. However, for something like snake, it isn't.

 This idea is what originally inspired me to make Trake. Trake isn't very graphically
 demanding, and while that is partly due to the simple graphics style, it is also due
 to the fact that the game doesn't completely redraw every frame. Instead, Trake takes
 what is currently in the buffer, modifies it, and sends it back to the monitor. This
 means that when a snake moves, the only parts of the frame that need to be modified
 are the front and back of the snake. This makes Trake extremely lightweight when it
 comes to graphical processing.


 I probably got the most enjoyment out of making the AI for Trake. In the AI's current
 state, it is fairly good at Tron, but very good at Snake. It could definitely be
 improved further, but doing so would actually make the AI too difficult to beat, in my
 opinion. As of now, I can usually beat it at Tron, but at Snake, I almost always lose.
 For me, this is great, because I like a challenge, which means that Trake is a game
 that keeps drawing me back for more playtime.

 The AI for Tron is quite simple. By default, it is in what's called "Defensive Mode", where
 it only turns when it absolutely has to. Each AI player is assigned a number of "Offensive
 Moves." At a random interval, if an AI has an "Offensive Move" left, it will change direction.
 Once the AI runs out of "Offensive Moves," it stays in "Defensive Mode" until it dies.

 In Snake, the AI has an "Offensive Mode" and a "Defensive Mode." By default, the AI is in
 "Defensive Mode." In this state, the AI will do whatever it needs to stay alive. It will
 occasionally turn at random intervals to mimic human behavior. When a pellet appears,
 the AI enters "Offensive Mode." In this state, the AI will do whatever it can to get to
 the pellet as fast as possible. It is important to note that the AI does not make use of
 screen wrapping in this state, so use that to your advantage.

 In both Snake and Tron, the before the AI makes its decision on which direction to go,
 it tries to determine which direction is the safest. It does this by running a quick
 simulation to determine if it could die within a certain number of moves after potentially
 taking that direction. This value is set to 100 in the game's code as of now, and it is tied
 to a variable called MAX_LOOK_AHEAD if anyone wishes to change it. The higher the value,
 the smarter the AI. However, I don't recommend too large of a number, as it will get too
 computationally demanding very quickly if the number is increased. Decreasing this number
 can significantly improve performance if any lag is experienced.




-============================- Troubleshooting and FAQ -============================-

 Q: How do I compile on Linux?
 A: If you have gcc and Allegro installed, it should be as simple as running the
    command "make" under the Linux folder.

 Q: How do I compile on Windows?
 A: Start by reading the included "How to compile Trake for Windows.txt".

 Q: What about Mac?
 A: I have no plans to support Trake on Mac. However, C++11 and Allegro 5 can be compiled
    on Mac, so it shouldn't be too hard to get a build environment set up and run the
    Makefile

 Q: The screen is flickering and the graphics are going crazy!!!
 A: This is technically a graphics driver bug. Trake relies on vsync and expects there
    to be only one graphical back buffer provided. It also expects this graphical buffer
    to not be touched by anything else. This is due to how Trake draws its frames, as it
    simply modifies the previous frame, rather than redrawing the whole thing.

 Q: What's with the weird framerate? Why doesn't this game simply run at my monitor's refresh rate?
 A: Again, this has to do with how Trake draws frames. A frame is only sent every time the snakes
    move. However, as the round lasts longer, the speed of the game increases, which means more
    frames. At the game's max speed, Trake runs at 60fps.

 Q: Help! I've unassigned an important key in the controls menu and now I can't play the game!
 A: Close the game with Alt+F4. Go to the game's directory and delete the file "TrakeSettings.cfg".
    Now, restart the game, and all of the settings will be returned to their default values.



