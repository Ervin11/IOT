# Embedded application template

## Usage
* Clone to `your-project-dir` and enter it

    ```
    git clone https://gitlab.com/catie_6tron/mbed-os-app-template.git YOUR_PROJECT_NAME
    cd YOUR_PROJECT_NAME
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

## Manage and save your project with Git:

* Edit readme file
* Modify remote URL to indicate your project URL with:

    ```sh
    $ git remote set-url origin git@gitlab.com:catie_sixtron/your-project-name.git'
    $ git push -u origin master
    ```