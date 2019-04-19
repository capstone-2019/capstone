// Modules to control application life and create native browser window
const {app, BrowserWindow, shell, Menu} = require('electron')

const os = require('os')
const { ipcMain } = require('electron');

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

function installMenu (window) {

    const template = [

        // dummy menu that is needed to make things render right on MacOS...
        {
            label: "",
        },

        // file menu
        {
            label: "File",
            submenu: [

                // save the current circuit into project directory
                {
                    label: "Save",
                    accelerator: "CmdOrCtrl+S",
                },

                // quit the application
                {
                    label: "Quit",
                    accelerator: "CmdOrCtrl+Q",
                    role: 'quit',
                },

                {
                    type: 'separator',
                },

                // import audio files
                {
                    label: "Import Audio",
                },

                // switch projects
                {
                    label: "Open Existing Project",
                },
            ],
        },

        // tools menu
        {
            label: "Tools",
            submenu: [

                // start a new simulation
                {
                    label: "Launch Simulation",
                    accelerator: "CmdOrCtrl+B",
                    click: function (menuItem, currentWindow) {
                        currentWindow.webContents.send('run-simulation');
                    }
                },

                // play sound from an output file
                {
                    label: "Play Sound",
                    click: function (menuItem, currentWindow) {
                        currentWindow.webContents.send('playback');
                    }
                }
            ],
        },

        // developer menu
        {
            label: "Developer",
            submenu: [
                {
                    label: "View DevTools",
                    accelerator: "CmdOrCtrl+J",
                    role: 'toggleDevTools',
                },
            ]
        },

    ];

    Menu.setApplicationMenu(Menu.buildFromTemplate(template));
}

function createWindow () {
    // Create the browser window.
    mainWindow = new BrowserWindow({
        width: 900,
        height: 900,
        webPreferences: { nodeIntegration: true },
    });

    // maximize the application
    mainWindow.maximize();

    // and load the index.html of the app.
    mainWindow.loadFile('index.html');

    // Emitted when the window is closed.
    mainWindow.on('closed', function () {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null
    });

    installMenu();
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow)

// Quit when all windows are closed.
app.on('window-all-closed', function () {
  // On macOS it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', function () {
  // On macOS it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (mainWindow === null) {
    createWindow()
  }
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
