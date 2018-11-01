open Maps;

type state = {
  /* We need a div for Google Maps to bind to. This provides it. */
  mapRef: ref(option(Dom.element)),
  map: option(Maps.Map.t),
};


type action = 
  | UpdateMap(Maps.Map.t);

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
  /* Function to be run on the creation of a Map */
  ~onMapCreated=((map : Map.t) => ()),
  /* Determines if directions are enabled */
  ~directionsEnabled=false,
  /* The next to props are only run if directions are enabled */
  /* Function to be run on the creation of a DirectionsService */
  ~onDirectionsServiceCreated=((ds : DirectionsService.t) => ()),
  /* Function to be run on the creation of a DirectionsDisplay */
  ~onDirectionsDisplayCreated=((dp : DirectionsDisplay.t) => ()),
  _children
) => {
    ...component,
    initialState: () => {
      mapRef: ref(None : option('a)),
      map: None
    },
    reducer: (action : action, state : state) => switch action {
      | UpdateMap(map) => ReasonReact.Update({...state, map: Some(map)})
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
                  map -> onMapCreated;
                  self.send(UpdateMap(map));
                  if (directionsEnabled) {
                    let directionsDisplay = DirectionsDisplay.create();
                    directionsDisplay -> DirectionsDisplay.setMap(map);
                    let directionsService = DirectionsService.create();
                    directionsService -> onDirectionsServiceCreated;
                    directionsDisplay -> onDirectionsDisplayCreated;
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
    render: (self) => {
      <div
        style=ReactDOMRe.Style.make(~height=height, ())
        ref={self.handle(setMapRef)}>
      </div>
    }
  };