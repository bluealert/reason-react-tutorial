let strToElm = ReasonReact.stringToElement;

type item = {
  title: string,
  completed: bool
};

type state = {
  items: list(item)
};

type action = 
  | AddItem;

let component = ReasonReact.reducerComponent("TodoApp");

let newItem = () => {title: "Click a button", completed: true};

let make = (_children) => {
  ...component,
  initialState: () => {
    items: [
      {title: "Write some things to do", completed: false}
    ]
  },
  reducer: (action, {items}) => 
    switch action {
    | AddItem => ReasonReact.Update({items: [newItem(), ...items]})
    },
  render: self => {
    let {items} = self.state;
    let numItems = List.length(items);
    <div className="app">
      <div className="title">
       (strToElm("What to do"))
       <button onClick=(self.reduce(_evt => AddItem))>
       (strToElm("Add something"))
       </button>
      </div>
      <div className="items"> (strToElm("Nothing")) </div>
      <div className="footer">
        (strToElm(string_of_int(numItems) ++ (numItems == 1 ? " item" : " items")))
      </div>
    </div>
  }
};
