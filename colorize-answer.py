#!/usr/bin/env python2.7
import sys
import argparse

ShellBrackets = ("\033[1;32m", "\033[0m")
HtmlBrackets = ('<span style="color:green">', '</span>')
OriginText = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum"

def parse_args():
    parser = argparse.ArgumentParser()
    parser.set_defaults(brackets=ShellBrackets, show_scores=True)
    parser.add_argument("--output-html", dest="brackets", action="store_const", const=HtmlBrackets)
    parser.add_argument("--output-term", dest="brackets", action="store_const", const=ShellBrackets)
    parser.add_argument("--hide-scores", dest="show_scores", action="store_const", const=False)
    return parser.parse_args()

def html_escape(c):
    if c == '"': return "&quote;"
    if c == '&': return "&amp;"
    if c == '<': return "&lt;"
    if c == '>': return "&gt;"
    return c


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
        out += html_escape(ct) if brackets == HtmlBrackets else ct
    if state == 1:
        out += brackets[1]
    return goodCharacters, out


if __name__ == "__main__":
    opts = parse_args()
    text = sys.stdin.readline().strip("\n")
    checked, vStr = colorize(text, opts.brackets)
    if opts.show_scores:
        print checked
    print vStr
    
