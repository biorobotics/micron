
# Instructions:

1. Set up the camera. First, set up firewire permissions as detailed on [this page](https://github.com/jhu-cisst/mechatronics-software/wiki/Development-Environment#32-set-permission-for-1394-device) The camera must be plugged into a firewire port. You should see the light blink green a few times and then stay on.

2. Install docker
    ```bash
    sudo apt-get install docker.io
    ```
3. Build container
    ```bash
    cd PATH_TO_THIS_FOLDER
    docker build -t micron:latest .
    ```
4. Run container
    ```bash
    docker run --name micron --network=host --privileged -it --device=/dev/fw1 micron:latest --rm
    ```

    You should see output like 

    ```
    Frame No. 12981 
    663154288: identified 1 marker(s)
    >> COOLCARD at (-29.97, -55.40, 357.99), rotation (degs): (-25.5, -12.8, -12.8) Warming up: camera not yet thermally stable
    Frame No. 12982 
    663154288: identified 0 marker(s)
    Frame No. 12983 
    663154288: identified 1 marker(s)
    >> COOLCARD at (-38.04, -55.29, 359.23), rotation (degs): (-27.1, -12.1, -12.1) Warming up: camera not yet thermally stable
    Frame No. 12984 
    663154288: identified 0 marker(s)
    ```

    If you only see `Frame No. XXXXX` without any `identified X marker(s)` just press `CTRL+C` to quit and try again

5. Add new markers by putting them in the `MTHome/Markers` folder

6. Add new cameras by putting their calibratoin files in the `MTHome/CalibrationFiles` folder

## Troubleshooting

* If the camera continues blinking forever, try booting the computer with the cable plugged in

* If the camera light doesn't turn green, your firewire adapter may be bad. We have had good luck with this firewire card ([PEX30009](https://www.amazon.com/Profile-PCI-Express-Firewire-Chipset-SD-PEX30009/dp/B002S53IG8))

* If the camera is not recognized or you get permission errors, try running ```sudo chmod a+rw /dev/fw*```

