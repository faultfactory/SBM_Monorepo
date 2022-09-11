# Crash Course on SW Development:

Git, CI/CD, CAN, Compilation. **Scary**.<BR>  Relax. You'll be fine. This is in no particular order because there wasn't a cohesive flow.

## Git: 
Git is decentralized source control that has dominated the software industry for years now. It allows multiple parties to work on the same content simultaneously and then bring that work back together. Do not attempt to use it for Simulink files. Only hand code. The project hinges on it. It will stretch your brain a bit but once you get it, you'll fall in love.

#### Git Terms:
- Repository: A copy of your code with its history, they exist locally (literally the directory you're in) and remotely in the form of a git server, such as github.  
- Cloning: getting files from a remote repository and making a copy of it with its history
- Staging: preparing file changes for submittion into the local repository's history. 
- Commit: A documented set of changes to the repository. 
- Push: Transferring commits made locally to the remote repository on the server. 
- Branches: Different versions of your code that allow you to take different paths for development. You can swap between them quite easily. 

Take a 20-30 minute video class on it somewhere and do a little practice. It will help you long term in your career. Even without a remote server, you can use git locally for scripts.<br> ```TODO: Find the team a good video / class / exercise```

You will need a github account and SBM will need to allow contributions from your account.

Tips: 
Commit early and often
Use pull requests to get buy-in from the team and document discussions.
Ask for help on slack if you need it.

## Github
I think it's worth noting that Github is just a company hosting git remote servers. They don't own the sw package ```git```. The value they do provide is in establishing some security gates, allowing code reviews via pull requests and a platform for hashing out discussions on code changes. A new feature they've rolled out is Github Actions which provides a basic form of [CI/CD](#cicd---continuous-integration--continuous-deployment).


## Mono-Repo

Just like it sounds: ```ONE REPO```. A mono-repo is a concept used to help align SW dependencies between various components in a project. Git has means of sharing code between repositories (```submodules```) but they are cumbersome and a bit much for SW newbies. <BR>
The biggest shared dependencies in any project involving software and hardware are typically communication related which is defined by the ```.dbc``` file. 
Using a mono-repo structure removes any issues caused by different versions being used by two parties. The correct version exists in the repository at that commit and nowhere else.

## CI/CD - Continuous Integration / Continuous Deployment

This is a big fancy word for "I automatically run tasks on someone else's computer". In the context of CI/CD typically that means building, testing, deploying, releasing etc. Github provides some basic functionality for this via Github Actions which we use for automating the creation of CAN .c and .h files. This removes managing a tool and process from your head. Just commit and push. 

In the future, we can establish a pipeline to automatically build the sketch files in the cloud to make sure nothing was broken by someone's work.



