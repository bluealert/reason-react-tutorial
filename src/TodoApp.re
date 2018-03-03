let strToElm = ReasonReact.stringToElement;

type item = {
  id: int,
  title: string,
  completed: bool
};

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, _children) => {
    ...component,
    render: (_self) => {
      <div className="item">
        <input _type="checkbox" checked=(Js.Boolean.to_js_boolean(item.completed)) />
        (strToElm(item.title))
      </div>
    }
  };
};

type state = {
  items: list(item)
};

type action = 
  | AddItem;

let component = ReasonReact.reducerComponent("TodoApp");

let lastId = ref(0);
let newItem = () => {
  lastId := lastId^ + 1;
  {id: lastId^, title: "Click a button", completed: true}
};

let make = (_children) => {
  ...component,
  initialState: () => {
    items: [
      {id: 0, title: "Write some things to do", completed: false}
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
      <div className="items"> (
        List.map((item) => <TodoItem key=(string_of_int(item.id)) item />, items)
         |> Array.of_list |> ReasonReact.arrayToElement
      )
      </div>
      <div className="footer">
        (strToElm(string_of_int(numItems) ++ (numItems == 1 ? " item" : " items")))
      </div>
    </div>
  }
};
