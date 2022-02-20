<p align="center"><b>Iris is an online eater and feeder.</b></p>

**Note:**

Iris will take far longer than what I anticipated, the first working version will be released under 1.0.0 tag.

---

# Progress

The speed is mostly limited to your internet connection and how fast cURL can give back the information. On my machine and with my Internet speed, this can take from **608ms** to up **1203ms**. **16ms-25ms** is the time used to scrape the given information.

Iris take a different approach to [tuxi] and [oi] as it will display any information that it can scrape. It leaves up to the user to do something with the information that is given to them, that's why each "method" will display its name before the information, as you can see below:

```
$ iris "Luna's Future lyrics"
GoogleLyrics I see a cold wind blowing through
GoogleLyrics I see days neither fun nor free
GoogleLyrics
GoogleLyrics I see a future caused by you
GoogleLyrics I see a path not meant to be
GoogleLyrics
GoogleLyrics The future should be filled with magic
GoogleLyrics Dreams and wishes brought to life
GoogleLyrics
GoogleLyrics But the days ahead are dark and tragic
GoogleLyrics No time for hope when all is strife
GoogleLyrics
GoogleLyrics Whatever might have been
GoogleLyrics All the dreams that ponies share
GoogleLyrics
GoogleLyrics Because of you, Snowfall Frost
GoogleLyrics Now the future is a cold nightmare
GoogleLyricsInformation Title Luna's Future
GoogleLyricsInformation Singer Princess Luna
GoogleLyricsInformation Source 提供元: Musixmatch
GoogleLyricsInformation Songwriters ソングライター: Daniel Luke Ingram
GoogleLyricsInformation Studios Luna's Future 歌詞 © Britteridge Publishing  Llc
```

The user then can simply choose to have some kind of information, for example, if he/she only wants lyrics, then just use:

```
$ iris "Luna's Future lyrics" | awk '$1 == "GoogleLyrics" { $1=""; print substr($0,2) }'
I see a cold wind blowing through
I see days neither fun nor free

I see a future caused by you
I see a path not meant to be

The future should be filled with magic
Dreams and wishes brought to life

But the days ahead are dark and tragic
No time for hope when all is strife

Whatever might have been
All the dreams that ponies share

Because of you, Snowfall Frost
Now the future is a cold nightmare
```

---

# Installation

The releases binaries can be get up [here](https://github.com/iris-cli/iris/releases).

If you want, however, build it to test yourself, you can. See [Makefile](/Makefile).

**Note:** I do not recommend you installing a single binary on your system or building just once the source code, because how information is displayed can often change, and for that often updates are necessary.

---

# Acknowledgements:

+ [tinyexpr](https://github.com/codeplea/tinyexpr/) -- Minimal expression evaluation engine as library.
+ [lexbor](https://github.com/lexbor/lexbor) -- Fast and powerful way to parse HTML.
+ [cURL](https://curl.se/) -- Minimal way to download a source page as library.
+ [tuxi](https://github.com/Bugswriter/tuxi/) -- Inspired by.
+ [oi](https://github.com/PureArtistry/oi/) -- As motivation to create a better assistant!

---

<p align="center"><a href="https://github.com/iris-cli/iris/blob/master/LICENSE">APACHE 2.0 LICENSE<a></p>
