# ImHex Oracle Plugin

Ever opened a unknown file and you just couldn't figure out what format it was in? Fear not! Simply ask the almightly Oracle for help!
![imhex_3rLetfxhTF](https://user-images.githubusercontent.com/10835354/205517339-d7986e96-a500-4632-b530-b9f5f1132697.gif)

## How does this work?

The plugin takes the first 0x100 bytes of your file (or all bytes if it's smaller than that) and sends them to OpenAI's ChatGPT (`text-davinci-002`) AI and asks it to identify the file format.
For this to work, you need to create an API Token in your OpenAI profile and enter it in ImHex's settings.

## Is this accurate?

Ehhhhh.... 
For many common file formats it's pretty accurate. It had no problems correctly identifying PE files, PNGs and WebAssembly (as seen above). However, if you feed it random garbage data or file formats it cannot know (such as proprietary file formats), it will output "I don't know" if you're lucky or just make up random bullshit if you're not.
