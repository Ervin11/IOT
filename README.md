# Embedded application template

## Usage

_Note: To clone **and** deploy the project use `mbed import https://gitlab.com/catie_6tron/mbed-os-app-template.git your-project-dir` and go directly to the target and toolchain definition._

* Clone to `your-project-dir` and enter it

    ```
    git clone https://gitlab.com/catie_6tron/mbed-os-app-template.git YOUR_PROJECT_DIR
    cd YOUR_PROJECT_DIR
    ```

* Create the mbed config file, otherwise Mbed CLI commands won't work:

    `touch .mbed` on Linux/macOS or `echo.> .mbed` on Windows

* Deploy Mbed OS with:

    ```
    mbed deploy
    ```

* Define your target and toolchain:

    ```
    mbed target ZEST_CORE_STM32L496RG
    mbed toolchain GCC_ARM
    ```

* Export to Eclipse IDE with:

    ```
    mbed export -i eclipse_6tron
    ```

## Compiling and programming without IDE

* Compile the project:

    ```
    mbed compile
    ```

* Program the target device with a J-Link debug probe, eg.:

    ```
    ./dist/program.py STM32L496RG BUILD/ZEST_CORE_STM32L496RG/GCC_ARM/your-project-name.elf
    ```

## Manage and save your project with Git:

* Edit readme file
* Modify remote URL to indicate your project URL with:

    ```sh
    $ git remote set-url origin YOUR-REPOSITORY-URL
    $ git push -u origin master
    ```
