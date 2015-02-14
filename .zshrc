#!/bin/zsh

autoload -Uz compinit zcalc
compinit
zstyle -e ':completion:*:default' list-colors 'reply=("${PREFIX:+=(#bi)($PREFIX:t)(?)*==02=01}:${(s.:.)LS_COLORS}")'
zstyle ':completion:*' menu select
zstyle '*:processes-names' command 'ps -e -o comm='
zstyle ':completion:*:warnings' format '%BSorry, no matches for: %d%b'
zstyle ':completion:*' file-sort modification reverse


autoload -U colors && colors
zstyle ':completion:*' list-colors "=(#b) #([0-9]#)*=31=36"


# User configuration
export EDITOR=vim
export PATH="/usr/local/bin:/usr/local/sbin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:$PATH"

export HOMEBREW_GITHUB_API_TOKEN=446ba6202ed46a44cf10bacd144a76528fbc88d4

export LSCOLORS=Exfxcxdxbxegedabagacad
#export MANPATH="/usr/local/man:/usr/pkg/man:$MANPATH"
# Aliases
alias scrot='screencapture -m'
alias ls='ls -G'
alias la='ls -a'
alias ll='ls -l'
alias ..='cd ..'
alias htop='sudo htop'
alias reboot='sudo reboot'
alias ping='ping -c 2'
alias speed='speedtest_cli'
alias music='ncmpcpp'
alias b4d='ssh b4dtr1p@b4dtr1p.tk -t tmux attach -t irc'
alias iotek='ssh b4dtr1p@vps.iotek.org -t tmux attach -t irc'
alias smc='/Applications/smcFanControl.app/Contents/Resources/smc'
alias gitpush='git push origin master && toilet -f future push_it_boss...'
alias gitpull='git pull origin master && toilet -f future pull_it_boss...'
alias gitnews='git fetch && git log ..origin/master'
alias ora='tty-clock -c -C 3 -B -D'
alias :q='sudo shutdown -h now'
alias ytdl='youtube-dl -t -x --audio-format mp3 --audio-quality 0'
alias screenrec=' ffmpeg -f avfoundation -s 1366x768 -an -r 16 -loglevel quiet -i 0:none -b:v 5M -y workflow-compil-${USER}.webm'


# Network/System monitoring
alias netlist='lsof -i -P | grep LISTEN' # show active network listener

#
# You may need to manually set your language environment
# export LANG=en_US.UTF-8


# Compilation flags
export ARCHFLAGS="-arch x86_64"

# ssh
# export SSH_KEY_PATH="~/.ssh/dsa_id"
#source /Users/b4d_tR1p/.iterm2_shell_integration.zsh

# prompt
#PROMPT="%{$fg[red]%}───╼ %{$reset_color%} "
PROMPT="%{$fg[red]%} » %{$reset_color%}"
RPROMPT="%B%{$fg[blue]%}%~%{$reset_color%}"
