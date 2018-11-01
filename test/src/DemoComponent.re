open BsGoogleMaps;

type state = {
  waypoints: array(Maps.waypoint),
  map: option(Maps.Map.t),
  directionsService: option(Maps.DirectionsService.t),
  directionsDisplay: option(Maps.DirectionsDisplay.t)
};

type actions =
  | UpdateWaypoints(array(Maps.waypoint))
  | UpdateMap(Maps.Map.t)
  | UpdateDirectionsService(Maps.DirectionsService.t)
  | UpdateDirectionsDisplay(Maps.DirectionsDisplay.t);

let component = ReasonReact.reducerComponent("DemoComponent");

let make(_children) {
  ...component,
  initialState: (self) => {
    waypoints: [||],
    map: None,
    directionsDisplay: None,
    directionsService: None
  },
  reducer: (action, state) => switch action {
    | UpdateWaypoints(waypoints) => ReasonReact.Update({...state, waypoints}) 
    | UpdateMap(map) => ReasonReact.Update({...state, map: Some(map)}) 
    | UpdateDirectionsService(directionsService) => ReasonReact.Update({...state, directionsService: Some(directionsService)}) 
    | UpdateDirectionsDisplay(directionsDisplay) => ReasonReact.Update({...state, directionsDisplay: Some(directionsDisplay)}) 
  },
  didUpdate: ({oldSelf, newSelf}) => {
    if (newSelf.state.directionsDisplay !== None && 
        newSelf.state.directionsService !== None &&
        newSelf.state.map != None) {
      let directionsDisplay = newSelf.state.directionsDisplay -> Belt.Option.getExn;
      let directionsService = newSelf.state.directionsService -> Belt.Option.getExn;
      let map = newSelf.state.map -> Belt.Option.getExn;
      let wpTest = Belt.Array.eq(oldSelf.state.waypoints, newSelf.state.waypoints, fun(x,y) => x == y);
      if (!wpTest) {
        let request = {
          "origin": "San Antonio, TX",
          "destination": "Atlanta, GA",
          "travelMode": "DRIVING",
          "waypoints": newSelf.state.waypoints  
        }
        directionsService -> Maps.DirectionsService.route(
          request,
          ((result, status) => {
            if (status == "OK") {
              directionsDisplay -> Maps.DirectionsDisplay.setDirections(result);
            }
          })
        );
      }
    }
  },
  render: (self) => {
    <div>
      <button
        onClick=((_evt) => self.send(UpdateWaypoints([|
          {"location": "College Park, GA", "stopover": true}
        |])))
        >
        (ReasonReact.string("Update Waypoints"))
      </button>
      <GoogleMap
        apiKey=""
        directionsEnabled=true
        onMapCreated=((map) => self.send(UpdateMap(map)))
        onDirectionsServiceCreated=((ds) => self.send(UpdateDirectionsService(ds)))
        onDirectionsDisplayCreated=((dp) => {
          self.send(UpdateDirectionsDisplay(dp));
          self.send(UpdateWaypoints([|
            {"location": "College Park, GA", "stopover": true},
            {"location": "Ragland, AL", "stopover": true}
          |]
          ));
        })
        >
      </GoogleMap>
    </div>
  }
}