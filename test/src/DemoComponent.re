open BsGoogleMaps

type state = {
  waypoints: array(Maps.waypoint)
};

type actions =
  | UpdateWaypoints(array(Maps.waypoint));

let component = ReasonReact.reducerComponent("DemoComponent");

let make(_children) {
  ...component,
  initialState: (self) => {
    waypoints: [|
      {"location": "College Park, GA", "stopover": true},
      {"location": "Ragland, AL", "stopover": true}
    |]
  },
  reducer: (action, state) => switch action {
    | UpdateWaypoints(waypoints) => ReasonReact.Update({...state, waypoints}) 
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
        enableDirections=true
        origin="Atlanta, GA"
        destination="Birmingham, AL"
        enableWaypoints=true
        waypoints=self.state.waypoints
        >
      </GoogleMap>
    </div>
  }
}