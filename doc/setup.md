# Setup your computer

## Before the journey

To take this course, you need to know Git and especially Github. So, I
provide below some useful reminders:

- [RTFM git](https://git-scm.com/docs)
- [I did something wrong and i don't know what to do](http://ohshitgit.com/)

## Get the good screwdriver (optional)

- Terminal : take the time to configure a productive tool

  - You can use your OS embedded terminal or use [Terminator](https://gnometerminator.blogspot.com/p/introduction.html) on Linux or [iTerm2](https://iterm2.com/) on macos
  - Use **zsh** instead of bash. If you start with zsh :

    - install zsh

    ```zsh
    # on Ubuntu
    apt-get install zsh

    # on OSX
    brew install zsh
    ```

    - install [Oh my zsh ](https://github.com/robbyrussell/oh-my-zsh)

    ```zsh
    # on Ubuntu
    sh -c "$(wget https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"

    # on OSX
    sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
    ```

    - change default shell : `chsh -s /bin/zsh`

- Text editor : install [visual studio code](https://code.visualstudio.com/Download)

  - You may use _Atom_, _VIM_, _Emacs_ or _Sublime Text_ if you're accurate with
    any of them. In this case, it's up to you to install the right plugins
    related to this training.
  - Coding must be _cool_ ! Install **Material icon theme** plugin and set color
    theme to **One Dark Pro**. Finally install [Fire Code](https://github.com/tonsky/FiraCode) fonts (yeah you now have a
    font with ligatures)
  - Alter code user parameters (menu : Code > Préférences > Paramètres ) to :

  ```json
    "editor.fontLigatures": true,
    "editor.fontFamily": "Fira Code",
  ```

  - On OSX, code distributed as .dmg so you need to add the binary to path (not necessary if you installed it with brew cask):

  ```zsh
  #on OSX
  echo "export PATH=/Applications/Visual\ Studio\ Code.app/Contents/Resources/app/bin:\$PATH" >> ~/.zshrc
  ```

  - Now you can open a project with

  ```zsh
  code path/to/my/project
  ```

  - Install [ReasonML plugin ](https://reasonml.github.io/docs/en/editor-plugins.html#native-project-development-community-supported) for your IDE

## Install native tooling

- Node.js : while reason tooling rely on npm we need node install.

```zsh
# on Ubuntu
wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.33.6/install.sh | bash

# on OSX
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.8/install.sh | bash

source ~/.zshrc #or source /.bash_rc if you didn't get zsh installed
nvm install --lts #install LTS version of node and related npm cli
```

- esy : [esy](https://esy.sh/en/) is a toolchain for native Reason and OCaml

```zsh
npm i -g esy
```

Now go back to [kata](./kata.md)
