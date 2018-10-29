open BsGoogleMaps.Maps;
open Webapi.Dom;


Js.Promise.(
  loadGoogleMapsApi(
    ~options={"key": ""},
    ()
  )
  |> then_(
    /* 
      All the Google Maps docs refer to this value as
      `google.maps`, so Ihere we call it maps. It's a
      Js object (Js.t({..}))
    */
    (maps : Js.t({..})) => {
      let node = switch (Document.getElementById("mapRaw", document)) {
        | Some(e) => e
        | None => Document.createElement("div", document)
      };
      let center : location = {"lat": -34, "lng": 150};
      let map = Map.create(
        ~element=node,
        ~options={
          "zoom": 8,
          "center": {"lat": -34, "lng": 150}
        },
        ()
      );
      let marker = Marker.create(
        {
          "position": center,
          "map": map
        }
      );
      let directionsDisplay = DirectionsDisplay.create();
      let directionsService = DirectionsService.create();
      let waypoints = [|
        {"location": "Adelaide, SA", "stopover": true},
        {"location": "Warburton, WA", "stopover": true}
      |];
      let request = {
        "origin": "Sydney, Australia",
        "destination": "Perth, Australia",
        "travelMode": "DRIVING",
        "waypoints": waypoints,
        "optimizeWaypoints": true
      }
      DirectionsDisplay.setMap(directionsDisplay, map);
      DirectionsService.route(
        directionsService,
        request,
        ((result, status) => {
          DirectionsDisplay.setDirections(directionsDisplay, result);
        })
      );
      () |> resolve;
    }
  )
)