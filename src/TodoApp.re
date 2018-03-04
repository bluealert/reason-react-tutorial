let strToElm = ReasonReact.stringToElement;

type item = {
  id: int,
  title: string,
  completed: bool
};

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, ~onToggle, _children) => {
    ...component,
    render: (_) =>
      <div className="item" onClick=(_evt => onToggle())>
        <input
          _type="checkbox"
          checked=(Js.Boolean.to_js_boolean(item.completed))
        />
        (strToElm(item.title))
      </div>
  };
};

module Input = {
  let evtToObj = evt =>
    evt |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
  let valFromEvt = evt : string => (evt |> evtToObj)##value;
  type state = string;
  let component = ReasonReact.reducerComponent("Input");
  let make = (~onSubmit, _) => {
    ...component,
    initialState: () => "",
    reducer: (newText, _text) => ReasonReact.Update(newText),
    render: ({state: text, reduce}) =>
      <input
        value=text
        _type="text"
        placeholder="Write something to do"
        onChange=(reduce(evt => valFromEvt(evt)))
        onKeyDown=(
          evt =>
            if (ReactEventRe.Keyboard.key(evt) == "Enter"
                && String.length(text) > 0) {
              onSubmit(text);
              (reduce(() => ""))();
            }
        )
      />
  };
};

type state = {items: list(item)};

type action =
  | AddItem(string)
  | ToggleItem(int);

let component = ReasonReact.reducerComponent("TodoApp");

let lastId = ref(0);

let newItem = title => {
  lastId := lastId^ + 1;
  {id: lastId^, title, completed: true};
};

let make = _children => {
  ...component,
  initialState: () => {
    items: [{id: 0, title: "Write some things to do", completed: false}]
  },
  reducer: (action, {items}) =>
    switch action {
    | AddItem(title) => ReasonReact.Update({items: [newItem(title), ...items]})
    | ToggleItem(id) =>
      let items =
        List.map(
          item =>
            item.id === id ? {...item, completed: ! item.completed} : item,
          items
        );
      ReasonReact.Update({items: items});
    },
  render: self => {
    let {items} = self.state;
    let numItems = List.length(items);
    <div className="app">
      <div className="title">
        (strToElm("What to do"))
        <Input onSubmit=(self.reduce(title => AddItem(title))) />
      </div>
      <div className="items">
        (
          List.map(
            item =>
              <TodoItem
                key=(string_of_int(item.id))
                onToggle=(self.reduce(() => ToggleItem(item.id)))
                item
              />,
            items
          )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
      <div className="footer">
        (
          strToElm(
            string_of_int(numItems) ++ (numItems == 1 ? " item" : " items")
          )
        )
      </div>
    </div>;
  }
};