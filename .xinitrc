cd $HOME

source $HOME/.profile

xrdb ~/.Xresources

xset +fp ~/.fonts
xset fp rehash
xset -b

xsetroot -cursor_name left_ptr

xmodmap ~/.Xmodmap
export LANG="en_US.UTF-8"
exec >>~/.xsession-errors 2>&1

mpd &
urxvtd &
sxhkd -c $HOME/.config/sxhkd/sxhkdrc -f 20 &
#exec bspwm -c $HOME/.config/bspwm/bspwmrc

swm &
exec urxvt -geometry 70x20 -e tmux
