## Simon Says

This is a PlatformIo project for the Esp32 chip, to play the game of Simon Says using 4 light, a buzzer and a phone.

This program hosts a page that can be access through the IP address displayed in the serial monitor (Just paste that address in your browser). To manage user input in "Real time", this page acts as a client to a server that uses websockets also hosted by the Esp32, which listens to user input events.