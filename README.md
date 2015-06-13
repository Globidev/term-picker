# term-picker
terminal item selector

Ncurses binding are not that great.
Doing it in good old C++ for now

## Try it out
With docker:
```
docker run --rm -it globidocker/term-picker-release item1 item2 item3
```
:warning: When using it in a non-tty environment (in a subshell for example), be sure to invoke it like this:
```
du -sh $(docker run --rm -e TERM -v $(tty):/dev/tty globidocker/term-picker-release *)
```
Here, we do not let Docker assign a pty. We manually share our current tty with the container. It is also necessary to share the `$TERM` variable
