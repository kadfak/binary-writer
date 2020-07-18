# binw

*Primitive tool that appends binary data to a file.*

## Build

```
./build.sh
```

## Usage

```
binw data.bin u8 123 str 'ding dong' i16 32145

xxd -b data.bin
01111011 01100100 01101001 01101110 01100111 00100000  {ding
01100100 01101111 01101110 01100111 10010001 01111101  dong.}
```

## License

[MIT](LICENSE)
