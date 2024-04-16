gcc encoder.c -o encoder -I./include -L./ -lopus -lm

gcc decoder.c -o decoder -I./include -L./ -lopus -lm