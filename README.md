# Woki-Toki

is basically a c application that emulates the real world woki-toki but instead of sending data through radio waves it uses the normal internet that we use on the daily basis. It uses an approach similar to Peer to Peer networking to acheive the communication between the 2 devices.

- currently i am planning to make it a crossplatform app (i hope so)

# Main Components
- using GTK+4 as a UI framework
- Audio Handling using SDL2 or might be SFML
- Networking using barkely sockets on linux and winsock2 on windows
- Threading using pthread on linux and processthreadsapi on windows
