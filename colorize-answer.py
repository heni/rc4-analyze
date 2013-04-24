#!/usr/bin/env python2.7
import sys

ShellBrackets = ("\033[1;31m", "\033[0m")
HtmlBrackets = ('<span class="checked">', '</span>')
OriginText = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum"


def colorize(text, brackets):
    state = 0
    goodCharacters = 0
    out = ""
    for ct, cm in zip(text, OriginText):
        if state == 0:
            if ct == cm:
                out += brackets[0]
                state = 1
        else:
            if ct != cm:
                out += brackets[1]
                state = 0
        goodCharacters += state
        out += ct
    if state == 1:
        out += brackets[1]
    return goodCharacters, out


if __name__ == "__main__":
    text = sys.stdin.readline().strip("\n")
    checked, vStr = colorize(text, ShellBrackets)
    print checked
    print vStr
    
