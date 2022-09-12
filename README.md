# SBM Monorepo

This was created to ease development of a compatible vehicle network for the team and get some members learning and understanding CAN and some basic software development techniques. The team's origins are grounded in Mechanical Engineering but the highest paid engineers in automotive do a lot of software now regardless of what their degree says.

My hope here is to help the team understand how this all works as well as grease the skids with some glue code, templates and cloud automation to allow you guys to focus on the actual domain content and getting data off the car quick with few limitations. 

[Important SW development concepts and tools relevant to this repo.](docs/crash_course.md)

[How to set up and use this repository as it is configured.](docs/using_this_repo.md)


# ```TODO```:
- Since the .dbc is stored here, data conversion tools should be alongside it to reference it.
- I would reccomend removing the years from your files so this repository can continue to live from year to year. The M4 CAN board is plenty fast and the eco-system within Arduino would allow you to port easily to new platforms if required. You can always create ```git tags``` to describe a point in time that something was done. 
    - Why? Assigning a year to something like this has the psychological impact of making the work seem stale and will drive people to abandon it and start fresh. Making tags on the releases allows the team to build on this from year to year.
- Create a README.md in the CAN_Demo_Node folder and encourage team to do the same. Write a document describing a good readme file with some links to how to use markdown. 

