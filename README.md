# memorize
Bible Memorization Assistant

## What is it?
Years ago, I came across an approach to memorizing texts verbatim wherein you write down the first letter of each word as a prompt.
So if I were trying to memorize "Four score and seven years ago..." I would write down "Fsasya", using each of those letters as a
prompt to help me remember the word. Once I have gone over it several times without error using just the letters, I start hiding
letters (or hide them altogether) to practice remembering the whole thing. This approach emphasizes "practicing remembering". (I wish
I knew where I read the article, because I would love to give credit for the approach).

The idea I had here was to create an application that would streamline this process for memorizing entire chapters of the Bible.

## What does it do?
This application - written as an NCURSES application - allows (or will allow) you to load a chapter of the Bible, progress through
verses, hiding and showing the full text of the verse at a push of a button, automatically generating the abbreviated text and allowing
you to mask the abbreviated text as you progress with your memorization of that verse.

## Philosophy of Biblical Memorization
I am very much a proponent of memorizing entire chapters - if not entire books - of the Bible. The reason for this is context: too
often, people spout nice sounding verses for encouragement but lack the understanding of their context and - hence - their meaning. So
while this program can be used for one off verse memorization, when it is complete I would encourage its users to memorize larger
passages.

## The Text
I am using the World English Bible as the basis for this project as it is open source and seems to be a fairly solid translation.
The source text I used (I BELIEVE I chose the British Version) can be found here: https://ebible.org/Scriptures/engwebpb_readaloud.zip
Naturally, I have had to make some edits to the first two lines of each file to streamline the texts integration with my program, and
to change the punctuation (namely double and single quotes) to a format more compatible with a terminal based application. While this
SHOULDN'T have any impact on the integrity of the text (unless I goof up somewhere in this process), it MAY mean that I cannot - by the
terms of the WEB - call it the World English Bible (I'll have to look more carefully at their terms of use for that).
