# SBM Monorepo

This was created to ease development of a compatible vehicle network for the team and get some members learning and understanding CAN and some basic software development techniques. The team's origins are grounded in Mechanical Engineering but the highest paid engineers in automotive do a lot of software now regardless of what their degree says.

My hope here is to help the team understand how this all works as well as grease the skids with some glue code, templates and cloud automation to allow you guys to focus on the actual domain content and getting data off the car quick with few limitations.

To lower the barrier of entry and to make 'first year' execution simpler, this is based on the assumption that each node on the network is a [Adafruit Feather M4 CAN.](https://www.adafruit.com/product/4759). Other platforms can be adapted easily but I picked as a starting point this for the following reasons: 
- Built-in CAN. No need to breadboard to test communications.
- Well stocked. Tough to get other boards with built-in CAN that are on the shelf. 
- Good eco system of boards for feather which makes it easier to prototype functionality.
- Plenty of speed and RAM so the team doesn't have to worry processing constraints while getting feet wet.
- Arduino eco system is familiar, and allows automation, no licenses. 

I'd like to eventually be transferring this to the ```stonybrookmotorsports``` github organization. Just want to continue making helpful tweaks and documentation updates without restriction for a while. Feel free to dabble.

### [Thoughts on why this may sense for career development and the team's future](docs/vehicle_networks.md)

### [Some basic SW development concepts and tools relevant to this repo.](docs/crash_course.md)

### [How to set up and use this repository as it is configured.](docs/using_this_repo.md)

### [Useful information on navigating and leveraging GitHub.](docs/github_magic.md)


# ```TODOs```:
- Since the .dbc is stored here, data conversion tools should be alongside it to reference it.
- Data ingestion:
    - Until the .dbc stabilizes need to define a means of tagging collected data with a SW release so we are using the correct reference when doing conversion.
    - ```TODO: Where does the team store it's data? Are there cloud accounts? AWS? Google Cloud? OneDrive? Azure?```
- So this repository can continue to live from year to year, remove years from the filenames. The M4 CAN board is plenty fast and the eco-system within Arduino would allow you to port easily to new platforms if required.
    - Why? Assigning a year to something like this has the psychological impact of making the work seem stale and will drive people to abandon it and start fresh. Making tags on the releases allows the team to build on this from year to year.
    - You can always create ```git tags``` to describe a point in time that something was done.
- The CAN boards make the SW easy but power and connectivity is still a bit of a mystery.
    - KiCad a Carrier Board with 12V power input. Should be achievable with 2 layer.
        - Power options:
            - DataAcq: battery is fine
            - Actively controlled elements: Engine power.
            - Race day: Unclear.