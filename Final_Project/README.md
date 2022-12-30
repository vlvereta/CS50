# CS50 Final Project: Camagru
#### Video Demo:  https://youtu.be/wnbUXlI5BjI
#### Description: Re-implementation of Snapchat on minimals.

## About
This project I created a few years ago in order my study in the French IT school 42. It was created with love and I believe it worth to be shared here with you. Not all desired functionality completed but the main idea is understandable.Also, I should say that the 42 school methodology restricts to use any external libraries which could make life easier and provide the needed functionality so the project was written on bare technologies: PHP and JS languages, MySQL database, Docker, other web and browser native technologies.<br>
So, what's project about. This app is like the Snapchat just for desktops. You have an ability to make photos from your webcam and add some filters and emojis..

## How to start
To start the project you should use docker as this way you are able to repeat the exact development infrastructure.
The command is: `docker-compose up -d`
This way the needed containers will run and the app will be accessible the link: `https://localhost:443`

## Main features
So, main features of the app is the following:
- Access to your webcam. Written on bare web technologies we're able to grant access to the laptop's webcam and make shots.
- Manually written filters that changes the preview and final picture.
- Ability to add emojis to pictures.
- General stuff:
    - Users management: creation, email verification, password validation and management;
    - Pictures management: creation, preview, deletion;
    - Feed: all pictures are visible to others, possibility to comment and like.

## Project structure
Folders:
1. **api:** backend of the application. It consists of common backend side structured folders - controllers, lib, models, services;
2. **client:** frontend of the application. It consists of common frontend side structured folders - views and components;
3. **config:** database configuration, email configuration.
4. **public:** public files that used in the app - assets, css, js;
5. Helper files: .gitignore, .htaccess, docker-compose.yml, Dockerfile, index.php, init.php.
As you can see the project mostly written on PHP and somewhere JS added a bit. As a helper phpMyAdmin was run on a separate docker container to make it easier control the MySQL database.

## Conclusion
There're even more ideas for improvements but it's getting to hard to maintain by 1 person and the actual goal isn't clear. I like the project but it was a bit tough write it from scratch using only bare technologies. I hope you will love it too!
