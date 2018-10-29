open Maps;

type state = {
  /* We need a div for Google Maps to bind to. This provides it. */
  mapRef: ref(option(Dom.element)),
  map: option(Maps.Map.t),
  origin: option(string),
  destination: option(string),
  travelMode: string,
  waypoints: option(array(waypoint)),
  directionsDisplay: option(Maps.DirectionsDisplay.t),
  directionsService: option(Maps.DirectionsService.t)
};

type action = 
  | UpdateMap(Maps.Map.t)
  | UpdateMapWithDirectionsDisplayAndService(Maps.Map.t, Maps.DirectionsDisplay.t, Maps.DirectionsService.t)
  | UpdateDirectionsDisplay(Maps.DirectionsDisplay.t)
  | UpdateDirectionsService(Maps.DirectionsService.t);

/* Function to be called by the main div */
let setMapRef = (theRef, {ReasonReact.state}) => {
  state.mapRef := Js.Nullable.toOption(theRef);
};
let component = ReasonReact.reducerComponent("GoogleMap");


let make = (
  /* We need an API key to access the maps. See the Maps API Documentation. */
  ~apiKey,
  /* Default height is 40em */
  ~height="40em",
  /* Default zoom level is 4 */
  ~zoom=4, 
  /* Center is Kansas City, MO, a good central place in the US */
  ~center=({"lat": 38.889931, "lng": -94.565559}),
  /* 
    Whether or not we will be using the Directions API.
    Creates a DirectionsService and DirectionsRender.
  */
  ~enableDirections=false,
  /* Whether or not to consider waypoints in directions */
  ~enableWaypoints=false,
  /* The origin point for directions */
  ~origin=?,
  /* The destination point for directions. */
  ~destination=?,
  /* The travel mode for directions. Defaults to driving. */
  ~travelMode="DRIVING",
  /* A list of waypoints for directions. */
  ~waypoints : option(array(waypoint)) = ?,
  _children
  ) => {
  {
    ...component,
    initialState: () => {
      mapRef: ref(None : option('a)),
      map: None,
      directionsDisplay: None,
      directionsService: None,
      origin: origin,
      destination: destination,
      travelMode: travelMode,
      waypoints: waypoints
    },
    reducer: (action : action, state : state) => switch action {
      | UpdateMap(map) => ReasonReact.Update({...state, map: Some(map)})
      | UpdateMapWithDirectionsDisplayAndService(map, directionsDisplay, directionsService) => 
        ReasonReact.Update({...state, map: Some(map), directionsDisplay: Some(directionsDisplay), directionsService: Some(directionsService)})
      | UpdateDirectionsDisplay(directionsDisplay) => ReasonReact.Update({...state, directionsDisplay: Some(directionsDisplay)})
      | UpdateDirectionsService(directionsService) => ReasonReact.Update({...state, directionsService: Some(directionsService)})
    },
    didMount: (self) => {
      if (!Maps.isGoogleLoaded()) {
        Js.Promise.(
          /* After it mounted, load the API */
          loadGoogleMapsApi(
            ~options={"key": apiKey},
            ()
          )
          |> then_(
            /* A map Js object is created... */
            (maps : Js.t({..})) => {
              /* 
                Needless but nessecary statement to get the element from the ref.
                It's needless because didMount is called **after** render, so
                we know the ref will contain Some(element).
              */
              switch (self.state.mapRef^) {
                | Some(elt) => {
                  /* Render the map to the div with the zoom and center previously defined */
                  let map = Map.create(
                    ~element=elt,
                    ~options={
                      "zoom": zoom,
                      "center": center
                    },
                    ()
                  );
                  if (enableDirections) {
                    let directionsService = DirectionsService.create();
                    let directionsDisplay = DirectionsDisplay.create();
                    directionsDisplay -> DirectionsDisplay.setMap(map);
                    self.send(
                      UpdateMapWithDirectionsDisplayAndService(map, directionsDisplay, directionsService)
                    );
                  } else {
                    self.send(UpdateMap(map));
                  };
                  ();
                }
                | None => ()
              } |> resolve;
            }
          )
        ) |> ignore;
      }
    },
    didUpdate: ({oldSelf, newSelf}) => {  
      if (enableDirections && newSelf.state.directionsDisplay != None && newSelf.state.directionsService != None) {
        let directionsService = Belt.Option.getExn(newSelf.state.directionsService);
        let directionsDisplay = Belt.Option.getExn(newSelf.state.directionsDisplay);
        let request = {
          "origin": Js.Nullable.fromOption(origin),
          "destination": Js.Nullable.fromOption(destination),
          "travelMode": travelMode,
          "waypoints": Js.Nullable.fromOption(waypoints),
          "optimizeWaypoints": enableWaypoints
        };
        directionsService -> DirectionsService.route(
          request,
          ((result, status) => {
            if (status == "OK") {
              directionsDisplay -> DirectionsDisplay.setDirections(result);
            }
          })
        )
      }
    },
    render: (self) => {
      <div
        style=ReactDOMRe.Style.make(~height=height, ())
        ref={self.handle(setMapRef)}>
      </div>
    }
  };
}