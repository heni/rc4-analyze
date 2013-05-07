rc4-analyze
===========

Analysis of RC4 crypto algorithm statistical deviations  
proof of concept for RC4 cryptoattack described at http://www.isg.rhul.ac.uk/tls/  
Read http://rutsh.blogspot.ru/2013/04/rc4-analysis.html (in russian) for more information

tools:
* **view-distribution**  - multithreaded Monte-Carlo key-stream distribution generator
* **encrypt-example**    - tool, which encrypt the same message with millions of different keys and periodically dump statistical files 
* **maxpos-decryptor**   - decrypting tool based on maximum frequency values
* **unigrams-decryptor** - decrypting tool based only on positional unigrams distribution
* **bigrams-detector**   - decrypting tool based on positional bigrams distribution

dependencies:
* gcc (>=4.7, I like c++11, really :)
* make
* fxt library (http://jjj.de/fxt/#fxt, you don't have to compile whole fxtlib just download and unpack it)
