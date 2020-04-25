# Protocol format

- All requests and responses are in JSON format 
- All requests have exactly 2 fields: `"command"` and `"arg"`
- All responses have at least 1 field: `"status"`
- Statuses have 3 possible values:
    - `200` - OK
    - `400` - Bad Request
    - `404` - Not Found
- Responses may have 1 additional field: `"result"`

All requests and responses are logged on the server side.

The list of commands follows.

### Add command

Request format:

```json
{
    "command": "add",
    "arg": {
        "name":   "Name",    // string, robot's name
        "price":  1000,      // int, robot's price
        "weight": 3.1415     // float, robot's weight
    }
}
```

Response format:

```json
{
    "status": 200,      // 200 if added
    "result": 18        // int, ID of the added robot
}
```

### Remove command

Request format:

```json
{
    "command": "remove",
    "arg":     18         // int, ID of the robot to remove
}
```

Response format:

```json
{
    "status": 200         // 200 if removed, 404 if does not exist
}
```

### Find command

Request format:

```json
{
    "command": "find",
    "arg":     18       // ID to search robot by
}
```

Response format:

```json
{
    "status": 200,     // 200 if found, 404 if not found
    // if status != 200, result field does not exist
    "result": {
    	"id":     18,        // int, ID of the found robot
    	"name":   "Name",    // string, Name of the found robot
    	"price":  1000,      // int, price of the found robot
    	"weight": 3.1415     // float, weight of the found robot
	}
}
```

### Update command

Request format:

```json
{
    "command": "update",
    "arg": {
        "id":     18,            // int, ID of the robot to update
        "name":   "Name",        // string, new name of the robot
        "price":  1000,          // int, new price of the robot
        "weight": 3.1415,        // float, new weight of the robot
    }
}
```

Response format:

```json
{
    "status": 200   // 200 if updated, 404 if does not exist (nothing to update)
}
```

### Search command

Request format:

```json
{
    "command": "find_all",
    "arg": {
        // all following fields are optional
        // if this is empty, matches all robots
        // otherwise, only robots with matching fields are returned
        "name":  "Name",     // string, optional
        "price":  1000,      // int, optional
        "weight": 3.1415     // float, optional
    }
}
```

Response format:

```json
{
    "status": 200,
    // result is array of matched robots
    // if none matched, result will be an empty array []
    "result": [
        {
            "id": 18,
            "name": "Robot1",
            "price": 1000,
            "weight": 3.1415
        },
        {
            "id": 37,
            "name": "Robot2",
            "price": 100500,
            "weight": 2.7182
        }
    ]
}
```

### Ping command

Request format:

```json
{
    "command": "ping",
    "arg": null
}
```

Response format:

```json
{
    "status": 200
}
```

