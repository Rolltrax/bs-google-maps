
module Map {
  type t;
  [@bs.new] external create : (~element : Dom.element, ~options : Js.t({..}), unit) => t = "google.maps.Map";
};

module Marker {
  type t;
  [@bs.new] external create : Js.t({..}) => t = "google.maps.Marker";
};


module DirectionsDisplay {
  type t;
  [@bs.new] external create : unit => t = "google.maps.DirectionsRenderer"
  [@bs.send] external setMap : (t, Map.t) => unit = "setMap";
  [@bs.send] external setDirections : (t, Js.t({..})) => unit = "setDirections";
};

module DirectionsService {
  type t;
  [@bs.new] external create : unit => t = "google.maps.DirectionsService"
  [@bs.send] external route : (t, Js.t({..}), (Js.t({..}), string) => unit) => unit = "route";
};

type location = {.
  "lat": int,
  "lng": int
};

type waypoint = {.
  "location": string,
  "stopover": bool
};


let isGoogleLoaded = [%raw {|
  function() {
    return typeof google !== "undefined"
  }  
|}];

[@bs.module]
external loadGoogleMapsApi : (
  ~options : Js.t({..})=?,
  unit
) => Js.Promise.t('a) = "load-google-maps-api";