# Install
## my env
 - Ubuntu 22.04
 - rpi4b
## requirements
```bash
apt-get install flex bison
```

## OS after Ubuntu 20.04
replace `SUBDIRS=$(PWD)` with `M=$(PWD)`

## make
```bash
sudo make
```

## insmod
```bash
chmod +x load-module
./load-module
```

