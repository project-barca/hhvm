(*
 * Copyright (c) 2019, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

type lsp_id =
  | NumberId of int
  | StringId of string

(* Note: this datatype provides no invariants that the string is well-formed. *)
type documentUri = DocumentUri of string [@@deriving eq]

val uri_of_string : string -> documentUri

val string_of_uri : documentUri -> string

type position = {
  line: int;
  character: int;
}
[@@deriving eq]

type range = {
  start: position;
  end_: position;
}
[@@deriving eq]

type textDocumentSaveReason =
  | Manual [@value 1]
  | AfterDelay [@value 2]
  | FocusOut [@value 3]
[@@deriving enum]

module Location : sig
  type t = {
    uri: documentUri;
    range: range;
  }
  [@@deriving eq]
end

module DefinitionLocation : sig
  type t = {
    location: Location.t;
    title: string option;
  }
end

type markedString =
  | MarkedString of string
  | MarkedCode of string * string

module Command : sig
  type t = {
    (* title of the command, like `save` *)
    title: string;
    (* the identifier of the actual command handler *)
    command: string;
    (* wire: it can be omitted *)
    arguments: Hh_json.json list;
  }
end

module TextEdit : sig
  type t = {
    range: range;
    newText: string;
  }
end

module TextDocumentIdentifier : sig
  type t = { uri: documentUri }
end

module VersionedTextDocumentIdentifier : sig
  type t = {
    uri: documentUri;
    version: int;
  }
end

module TextDocumentEdit : sig
  type t = {
    textDocument: VersionedTextDocumentIdentifier.t;
    edits: TextEdit.t list;
  }
end

module WorkspaceEdit : sig
  type t = {
    changes: TextEdit.t list SMap.t; (* holds changes to existing docs *)
  }
end

module TextDocumentItem : sig
  type t = {
    uri: documentUri;
    languageId: string;
    version: int;
    text: string;
  }
end

module CodeLens : sig
  type t = {
    range: range;
    command: Command.t;
    data: Hh_json.json option;
  }
end

module TextDocumentPositionParams : sig
  type t = {
    textDocument: TextDocumentIdentifier.t;
    position: position;
  }
end

module DocumentFilter : sig
  type t = {
    language: string option;
    scheme: string option;
    pattern: string option;
  }
end

module DocumentSelector : sig
  type t = DocumentFilter.t list
end

module SymbolInformation : sig
  type symbolKind =
    | File [@value 1]
    | Module [@value 2]
    | Namespace [@value 3]
    | Package [@value 4]
    | Class [@value 5]
    | Method [@value 6]
    | Property [@value 7]
    | Field [@value 8]
    | Constructor [@value 9]
    | Enum [@value 10]
    | Interface [@value 11]
    | Function [@value 12]
    | Variable [@value 13]
    | Constant [@value 14]
    | String [@value 15]
    | Number [@value 16]
    | Boolean [@value 17]
    | Array [@value 18]
    | Object [@value 19]
    | Key [@value 20]
    | Null [@value 21]
    | EnumMember [@value 22]
    | Struct [@value 23]
    | Event [@value 24]
    | Operator [@value 25]
    | TypeParameter [@value 26]
  [@@deriving enum]

  type t = {
    name: string;
    kind: symbolKind;
    location: Location.t;
    containerName: string option;
  }
end

module CallHierarchyItem : sig
  type t = {
    name: string;
    kind: SymbolInformation.symbolKind;
    detail: string option;
    uri: documentUri;
    range: range;
    selectionRange: range;
  }
end

module CallHierarchyCallsRequestParam : sig
  type t = { item: CallHierarchyItem.t }
end

module MessageType : sig
  type t =
    | ErrorMessage [@value 1]
    | WarningMessage [@value 2]
    | InfoMessage [@value 3]
    | LogMessage [@value 4]
  [@@deriving eq, enum]
end

module CancelRequest : sig
  type params = cancelParams

  and cancelParams = { id: lsp_id }
end

module SetTraceNotification : sig
  type params =
    | Verbose
    | Off
end

module CodeActionKind : sig
  type t = string * string list

  val is_kind : t -> t -> bool

  val contains_kind : t -> t list -> bool

  val contains_kind_opt : default:bool -> t -> t list option -> bool

  val kind_of_string : string -> t

  val string_of_kind : t -> string

  val sub_kind : t -> string -> t

  val quickfix : t

  val refactor : t

  val source : t
end

module Initialize : sig
  type textDocumentSyncKind =
    | NoSync [@value 0]
    | FullSync [@value 1]
    | IncrementalSync [@value 2]
  [@@deriving enum]

  type params = {
    processId: int option;
    rootPath: string option;
    rootUri: documentUri option;
    initializationOptions: initializationOptions;
    client_capabilities: client_capabilities;
    trace: trace;
  }

  and result = { server_capabilities: server_capabilities }

  and errorData = { retry: bool }

  and trace =
    | Off
    | Messages
    | Verbose

  (** These are hack-specific options. They're all optional in initialize request,
  and we pick a default if necessary while parsing. *)
  and initializationOptions = {
    namingTableSavedStatePath: string option;
        (** used for test scenarios where we pass a naming-table sqlite file,
        rather than leaving clientIdeDaemon to find and download one itself. *)
    namingTableSavedStateTestDelay: float;
        (** used for test scenarios where we've passed
        a naming-sqlite file and so clientIdeDaemon completes instantly, but we still want
        a little bit of a delay before it reports readiness so as to exercise race conditions.
        This is the delay in seconds. *)
    delayUntilDoneInit: bool;
        (** used for test scenarios where we want clientLsp to delay receiving any
        further LSP requests until clientIdeDaemon has done its init. *)
  }

  and client_capabilities = {
    workspace: workspaceClientCapabilities;
    textDocument: textDocumentClientCapabilities;
    window: windowClientCapabilities;
    telemetry: telemetryClientCapabilities;
  }

  and workspaceClientCapabilities = {
    applyEdit: bool;
    workspaceEdit: workspaceEdit;
    didChangeWatchedFiles: dynamicRegistration;
  }

  and dynamicRegistration = { dynamicRegistration: bool }

  and workspaceEdit = { documentChanges: bool }

  and textDocumentClientCapabilities = {
    synchronization: synchronization;
    completion: completion;
    codeAction: codeAction;
    definition: definition;
    typeDefinition: typeDefinition;
    declaration: declaration;
    implementation: implementation;
  }

  and synchronization = {
    can_willSave: bool;
    can_willSaveWaitUntil: bool;
    can_didSave: bool;
  }

  and completion = { completionItem: completionItem }

  and completionItem = { snippetSupport: bool }

  and codeAction = {
    codeAction_dynamicRegistration: bool;
    codeActionLiteralSupport: codeActionliteralSupport option;
  }

  and codeActionliteralSupport = { codeAction_valueSet: CodeActionKind.t list }

  and definition = { definitionLinkSupport: bool }

  and typeDefinition = { typeDefinitionLinkSupport: bool }

  and declaration = { declarationLinkSupport: bool }

  and implementation = { implementationLinkSupport: bool }

  and windowClientCapabilities = { status: bool }

  and telemetryClientCapabilities = { connectionStatus: bool }

  and server_capabilities = {
    textDocumentSync: textDocumentSyncOptions;
    hoverProvider: bool;
    completionProvider: completionOptions option;
    signatureHelpProvider: signatureHelpOptions option;
    definitionProvider: bool;
    typeDefinitionProvider: bool;
    referencesProvider: bool;
    callHierarchyProvider: bool;
    documentHighlightProvider: bool;
    documentSymbolProvider: bool;
    workspaceSymbolProvider: bool;
    codeActionProvider: bool;
    codeLensProvider: codeLensOptions option;
    documentFormattingProvider: bool;
    documentRangeFormattingProvider: bool;
    documentOnTypeFormattingProvider: documentOnTypeFormattingOptions option;
    renameProvider: bool;
    documentLinkProvider: documentLinkOptions option;
    executeCommandProvider: executeCommandOptions option;
    implementationProvider: bool;
    typeCoverageProviderFB: bool;
    rageProviderFB: bool;
  }

  and completionOptions = {
    resolveProvider: bool;
    completion_triggerCharacters: string list;
  }

  and signatureHelpOptions = { sighelp_triggerCharacters: string list }

  and codeLensOptions = { codelens_resolveProvider: bool }

  and documentOnTypeFormattingOptions = {
    firstTriggerCharacter: string;
    moreTriggerCharacter: string list;
  }

  and documentLinkOptions = { doclink_resolveProvider: bool }

  and executeCommandOptions = { commands: string list }

  and textDocumentSyncOptions = {
    want_openClose: bool;
    want_change: textDocumentSyncKind;
    want_willSave: bool;
    want_willSaveWaitUntil: bool;
    want_didSave: saveOptions option;
  }

  and saveOptions = { includeText: bool }
end

module RageFB : sig
  type result = rageItem list

  and rageItem = {
    title: string option;
    data: string;
  }
end

module CodeLensResolve : sig
  type params = CodeLens.t

  and result = CodeLens.t
end

module Hover : sig
  type params = TextDocumentPositionParams.t

  and result = hoverResult option

  and hoverResult = {
    contents: markedString list;
    range: range option;
  }
end

module PublishDiagnostics : sig
  type diagnosticCode =
    | IntCode of int
    | StringCode of string
    | NoCode
  [@@deriving eq]

  type diagnosticSeverity =
    | Error
    | Warning
    | Information
    | Hint
  [@@deriving enum, eq]

  type params = publishDiagnosticsParams

  and publishDiagnosticsParams = {
    uri: documentUri;
    diagnostics: diagnostic list;
    isStatusFB: bool;
        (** FB-specific extension, for diagnostics used only to show status *)
  }

  and diagnostic = {
    range: range;
    severity: diagnosticSeverity option;
    code: diagnosticCode;
    source: string option;
    message: string;
    relatedInformation: diagnosticRelatedInformation list;
    relatedLocations: relatedLocation list;
  }
  [@@deriving eq]

  and diagnosticRelatedInformation = {
    relatedLocation: Location.t;
    relatedMessage: string;
  }
  [@@deriving eq]

  and relatedLocation = diagnosticRelatedInformation
end

module DidOpen : sig
  type params = didOpenTextDocumentParams

  and didOpenTextDocumentParams = { textDocument: TextDocumentItem.t }
end

module DidClose : sig
  type params = didCloseTextDocumentParams

  and didCloseTextDocumentParams = { textDocument: TextDocumentIdentifier.t }
end

module DidSave : sig
  type params = didSaveTextDocumentParams

  and didSaveTextDocumentParams = {
    textDocument: TextDocumentIdentifier.t;
    text: string option;
  }
end

module DidChange : sig
  type params = didChangeTextDocumentParams

  and didChangeTextDocumentParams = {
    textDocument: VersionedTextDocumentIdentifier.t;
    contentChanges: textDocumentContentChangeEvent list;
  }

  and textDocumentContentChangeEvent = {
    range: range option;
    rangeLength: int option;
    text: string;
  }
end

module WillSaveWaitUntil : sig
  type params = willSaveWaitUntilTextDocumentParams

  and willSaveWaitUntilTextDocumentParams = {
    textDocument: TextDocumentIdentifier.t;
    reason: textDocumentSaveReason;
  }

  and result = TextEdit.t list
end

module DidChangeWatchedFiles : sig
  type registerOptions = { watchers: fileSystemWatcher list }

  and fileSystemWatcher = { globPattern: string }

  type fileChangeType =
    | Created
    | Updated
    | Deleted
  [@@deriving enum]

  type params = { changes: fileEvent list }

  and fileEvent = {
    uri: documentUri;
    type_: fileChangeType;
  }
end

module Definition : sig
  type params = TextDocumentPositionParams.t

  and result = DefinitionLocation.t list
end

module TypeDefinition : sig
  type params = TextDocumentPositionParams.t

  and result = DefinitionLocation.t list
end

module Implementation : sig
  type params = TextDocumentPositionParams.t

  and result = Location.t list
end

module CodeAction : sig
  type t = {
    title: string;
    kind: CodeActionKind.t;
    diagnostics: PublishDiagnostics.diagnostic list;
    action: edit_and_or_command;
  }

  and edit_and_or_command =
    | EditOnly of WorkspaceEdit.t
    | CommandOnly of Command.t
    | BothEditThenCommand of (WorkspaceEdit.t * Command.t)

  type result = command_or_action list

  and command_or_action =
    | Command of Command.t
    | Action of t
end

module CodeActionRequest : sig
  type params = {
    textDocument: TextDocumentIdentifier.t;
    range: range;
    context: codeActionContext;
  }

  and codeActionContext = {
    diagnostics: PublishDiagnostics.diagnostic list;
    only: CodeActionKind.t list option;
  }
end

module Completion : sig
  type completionItemKind =
    | Text (* 1 *)
    | Method (* 2 *)
    | Function (* 3 *)
    | Constructor (* 4 *)
    | Field (* 5 *)
    | Variable (* 6 *)
    | Class (* 7 *)
    | Interface (* 8 *)
    | Module (* 9 *)
    | Property (* 10 *)
    | Unit (* 11 *)
    | Value (* 12 *)
    | Enum (* 13 *)
    | Keyword (* 14 *)
    | Snippet (* 15 *)
    | Color (* 16 *)
    | File (* 17 *)
    | Reference (* 18 *)
    | Folder (* 19 *)
    | MemberOf (* 20 *)
    | Constant (* 21 *)
    | Struct (* 22 *)
    | Event (* 23 *)
    | Operator (* 24 *)
    | TypeParameter (* 25 *)
  [@@deriving enum]

  type insertTextFormat =
    | PlainText (* 1 *)
    (* the insertText/textEdits are just plain strings *)
    | SnippetFormat (* 2 *)
  (* wire: just "Snippet" *)
  [@@deriving enum]

  type completionTriggerKind =
    | Invoked [@value 1]
    | TriggerCharacter [@value 2]
    | TriggerForIncompleteCompletions [@value 3]
  [@@deriving enum]

  val is_invoked : completionTriggerKind -> bool

  type params = completionParams

  and completionParams = {
    loc: TextDocumentPositionParams.t;
    context: completionContext option;
  }

  and completionContext = {
    triggerKind: completionTriggerKind;
    triggerCharacter: string option;
  }

  and result = completionList

  (* wire: can also be 'completionItem list' *)
  and completionList = {
    isIncomplete: bool;
    (* further typing should result in recomputing *)
    items: completionItem list;
  }

  and completionDocumentation =
    | MarkedStringsDocumentation of markedString list
    | UnparsedDocumentation of Hh_json.json

  and completionItem = {
    label: string;
    (* the label in the UI *)
    kind: completionItemKind option;
    (* tells editor which icon to use *)
    detail: string option;
    (* human-readable string like type/symbol info *)
    inlineDetail: string option;
    (* nuclide-specific, right column *)
    itemType: string option;
    (* nuclide-specific, left column *)
    documentation: completionDocumentation option;
    (* human-readable doc-comment *)
    sortText: string option;
    (* used for sorting; if absent, uses label *)
    filterText: string option;
    (* used for filtering; if absent, uses label *)
    insertText: string option;
    (* used for inserting; if absent, uses label *)
    insertTextFormat: insertTextFormat option;
    textEdits: TextEdit.t list;
    (* wire: split into hd and tl *)
    command: Command.t option;
    (* if present, is executed after completion *)
    data: Hh_json.json option;
  }
end

module CompletionItemResolve : sig
  type params = Completion.completionItem

  and result = Completion.completionItem
end

module WorkspaceSymbol : sig
  type params = workspaceSymbolParams

  and result = SymbolInformation.t list

  and workspaceSymbolParams = { query: string }
end

module DocumentSymbol : sig
  type params = documentSymbolParams

  and result = SymbolInformation.t list

  and documentSymbolParams = { textDocument: TextDocumentIdentifier.t }
end

module FindReferences : sig
  type params = referenceParams

  and result = Location.t list

  and referenceParams = {
    loc: TextDocumentPositionParams.t;
    context: referenceContext;
  }

  and referenceContext = {
    includeDeclaration: bool;
    includeIndirectReferences: bool;
  }
end

module PrepareCallHierarchy : sig
  type params = TextDocumentPositionParams.t

  type result = CallHierarchyItem.t list option
end

module CallHierarchyIncomingCalls : sig
  type params = CallHierarchyCallsRequestParam.t

  type result = callHierarchyIncomingCall list option

  and callHierarchyIncomingCall = {
    from: CallHierarchyItem.t;
    fromRanges: range list;
  }
end

module CallHierarchyOutgoingCalls : sig
  type params = CallHierarchyCallsRequestParam.t

  type result = callHierarchyOutgoingCall list option

  and callHierarchyOutgoingCall = {
    (* The name should just be "to", but "to" is a reserved keyword in OCaml*)
    call_to: CallHierarchyItem.t;
    fromRanges: range list;
  }
end

module DocumentHighlight : sig
  type params = TextDocumentPositionParams.t

  type documentHighlightKind =
    | Text [@value 1]
    | Read [@value 2]
    | Write [@value 3]
  [@@deriving enum]

  type result = documentHighlight list

  and documentHighlight = {
    range: range;
    kind: documentHighlightKind option;
  }
end

module TypeCoverageFB : sig
  type params = typeCoverageParams

  and result = {
    coveredPercent: int;
    uncoveredRanges: uncoveredRange list;
    defaultMessage: string;
  }

  and typeCoverageParams = { textDocument: TextDocumentIdentifier.t }

  and uncoveredRange = {
    range: range;
    message: string option;
  }
end

module ToggleTypeCoverageFB : sig
  type params = toggleTypeCoverageParams

  and toggleTypeCoverageParams = { toggle: bool }
end

module DocumentFormatting : sig
  type params = documentFormattingParams

  and result = TextEdit.t list

  and documentFormattingParams = {
    textDocument: TextDocumentIdentifier.t;
    options: formattingOptions;
  }

  and formattingOptions = {
    tabSize: int;
    insertSpaces: bool;
  }
end

module DocumentRangeFormatting : sig
  type params = documentRangeFormattingParams

  and result = TextEdit.t list

  and documentRangeFormattingParams = {
    textDocument: TextDocumentIdentifier.t;
    range: range;
    options: DocumentFormatting.formattingOptions;
  }
end

module DocumentOnTypeFormatting : sig
  type params = documentOnTypeFormattingParams

  and result = TextEdit.t list

  and documentOnTypeFormattingParams = {
    textDocument: TextDocumentIdentifier.t;
    position: position;
    ch: string;
    options: DocumentFormatting.formattingOptions;
  }
end

module SignatureHelp : sig
  type params = TextDocumentPositionParams.t

  and result = t option

  and t = {
    signatures: signature_information list;
    activeSignature: int;
    activeParameter: int;
  }

  and signature_information = {
    siginfo_label: string;
    siginfo_documentation: string option;
    parameters: parameter_information list;
  }

  and parameter_information = {
    parinfo_label: string;
    parinfo_documentation: string option;
  }
end

module Rename : sig
  type params = renameParams

  and result = WorkspaceEdit.t

  and renameParams = {
    textDocument: TextDocumentIdentifier.t;
    position: position;
    newName: string;
  }
end

module DocumentCodeLens : sig
  type params = codelensParams

  and result = CodeLens.t list

  and codelensParams = { textDocument: TextDocumentIdentifier.t }
end

module LogMessage : sig
  type params = logMessageParams

  and logMessageParams = {
    type_: MessageType.t;
    message: string;
  }
end

module ShowMessage : sig
  type params = showMessageParams

  and showMessageParams = {
    type_: MessageType.t;
    message: string;
  }
end

module ShowMessageRequest : sig
  type t =
    | Present of { id: lsp_id }
    | Absent

  and params = showMessageRequestParams

  and result = messageActionItem option

  and showMessageRequestParams = {
    type_: MessageType.t;
    message: string;
    actions: messageActionItem list;
  }

  and messageActionItem = { title: string }
end

module ShowStatusFB : sig
  type params = showStatusParams

  and result = ShowMessageRequest.messageActionItem option

  (** the showStatus LSP request will be handled by our VSCode extension.
  It's a facebook-specific extension to the LSP spec. How it's rendered
  is currently [shortMessage] in the status-bar, and "[progress]/[total] [message]"
  in the tooltip. The [telemetry] field isn't displayed to the user, but might
  be useful to someone debugging an LSP transcript. *)
  and showStatusParams = {
    request: ShowMessageRequest.showMessageRequestParams;
    progress: int option;
    total: int option;
    shortMessage: string option;
    telemetry: Hh_json.json option;
  }
end

module ConnectionStatusFB : sig
  type params = connectionStatusParams

  and connectionStatusParams = { isConnected: bool }
end

module Error : sig
  type code =
    | ParseError [@value -32700]
    | InvalidRequest [@value -32600]
    | MethodNotFound [@value -32601]
    | InvalidParams [@value -32602]
    | InternalError [@value -32603]
    | ServerErrorStart [@value -32099]
    | ServerErrorEnd [@value -32000]
    | ServerNotInitialized [@value -32002]
    | UnknownErrorCode [@value -32001]
    | RequestCancelled [@value -32800]
    | ContentModified [@value -32801]
  [@@deriving show, enum]

  type t = {
    code: code;
    message: string;
    data: Hh_json.json option;
  }

  exception LspException of t
end

type lsp_registration_options =
  | DidChangeWatchedFilesRegistrationOptions of
      DidChangeWatchedFiles.registerOptions

module RegisterCapability : sig
  type params = { registrations: registration list }

  and registration = {
    id: string;
    method_: string;
    registerOptions: lsp_registration_options;
  }

  val make_registration : lsp_registration_options -> registration
end

type lsp_request =
  | InitializeRequest of Initialize.params
  | RegisterCapabilityRequest of RegisterCapability.params
  | ShutdownRequest
  | CodeLensResolveRequest of CodeLensResolve.params
  | HoverRequest of Hover.params
  | DefinitionRequest of Definition.params
  | TypeDefinitionRequest of TypeDefinition.params
  | ImplementationRequest of Implementation.params
  | CodeActionRequest of CodeActionRequest.params
  | CompletionRequest of Completion.params
  | CompletionItemResolveRequest of CompletionItemResolve.params
  | WorkspaceSymbolRequest of WorkspaceSymbol.params
  | DocumentSymbolRequest of DocumentSymbol.params
  | FindReferencesRequest of FindReferences.params
  | PrepareCallHierarchyRequest of PrepareCallHierarchy.params
  | CallHierarchyIncomingCallsRequest of CallHierarchyIncomingCalls.params
  | CallHierarchyOutgoingCallsRequest of CallHierarchyOutgoingCalls.params
  | DocumentHighlightRequest of DocumentHighlight.params
  | TypeCoverageRequestFB of TypeCoverageFB.params
  | DocumentFormattingRequest of DocumentFormatting.params
  | DocumentRangeFormattingRequest of DocumentRangeFormatting.params
  | DocumentOnTypeFormattingRequest of DocumentOnTypeFormatting.params
  | ShowMessageRequestRequest of ShowMessageRequest.params
  | ShowStatusRequestFB of ShowStatusFB.params
  | RageRequestFB
  | RenameRequest of Rename.params
  | DocumentCodeLensRequest of DocumentCodeLens.params
  | SignatureHelpRequest of SignatureHelp.params
  | HackTestStartServerRequestFB
  | HackTestStopServerRequestFB
  | HackTestShutdownServerlessRequestFB
  | WillSaveWaitUntilRequest of WillSaveWaitUntil.params
  | UnknownRequest of string * Hh_json.json option

type lsp_result =
  | InitializeResult of Initialize.result
  | ShutdownResult
  | CodeLensResolveResult of CodeLensResolve.result
  | HoverResult of Hover.result
  | DefinitionResult of Definition.result
  | TypeDefinitionResult of TypeDefinition.result
  | ImplementationResult of Implementation.result
  | CodeActionResult of CodeAction.result
  | CompletionResult of Completion.result
  | CompletionItemResolveResult of CompletionItemResolve.result
  | WorkspaceSymbolResult of WorkspaceSymbol.result
  | DocumentSymbolResult of DocumentSymbol.result
  | FindReferencesResult of FindReferences.result
  | PrepareCallHierarchyResult of PrepareCallHierarchy.result
  | CallHierarchyIncomingCallsResult of CallHierarchyIncomingCalls.result
  | CallHierarchyOutgoingCallsResult of CallHierarchyOutgoingCalls.result
  | DocumentHighlightResult of DocumentHighlight.result
  | TypeCoverageResultFB of TypeCoverageFB.result
  | DocumentFormattingResult of DocumentFormatting.result
  | DocumentRangeFormattingResult of DocumentRangeFormatting.result
  | DocumentOnTypeFormattingResult of DocumentOnTypeFormatting.result
  | ShowMessageRequestResult of ShowMessageRequest.result
  | ShowStatusResultFB of ShowStatusFB.result
  | RageResultFB of RageFB.result
  | RenameResult of Rename.result
  | DocumentCodeLensResult of DocumentCodeLens.result
  | SignatureHelpResult of SignatureHelp.result
  | HackTestStartServerResultFB
  | HackTestStopServerResultFB
  | HackTestShutdownServerlessResultFB
  | RegisterCapabilityRequestResult
  | WillSaveWaitUntilResult of WillSaveWaitUntil.result
  | ErrorResult of Error.t

type lsp_notification =
  | ExitNotification
  | CancelRequestNotification of CancelRequest.params
  | PublishDiagnosticsNotification of PublishDiagnostics.params
  | DidOpenNotification of DidOpen.params
  | DidCloseNotification of DidClose.params
  | DidSaveNotification of DidSave.params
  | DidChangeNotification of DidChange.params
  | DidChangeWatchedFilesNotification of DidChangeWatchedFiles.params
  | LogMessageNotification of LogMessage.params
  | TelemetryNotification of LogMessage.params * (string * Hh_json.json) list
      (** For telemetry, LSP allows 'any', but we're going to send these *)
  | ShowMessageNotification of ShowMessage.params
  | ConnectionStatusNotificationFB of ConnectionStatusFB.params
  | InitializedNotification
  | SetTraceNotification of SetTraceNotification.params
  | LogTraceNotification (* $/logTraceNotification *)
  | ToggleTypeCoverageNotificationFB of ToggleTypeCoverageFB.params
  | UnknownNotification of string * Hh_json.json option

type lsp_message =
  | RequestMessage of lsp_id * lsp_request
  | ResponseMessage of lsp_id * lsp_result
  | NotificationMessage of lsp_notification

type 'a lsp_handler = 'a lsp_result_handler * 'a lsp_error_handler

and 'a lsp_error_handler = Error.t * string -> 'a -> 'a

and 'a lsp_result_handler =
  | ShowMessageHandler of (ShowMessageRequest.result -> 'a -> 'a)
  | ShowStatusHandler of (ShowStatusFB.result -> 'a -> 'a)

module IdKey : sig
  type t = lsp_id

  val compare : t -> t -> int
end

module IdSet : sig
  include module type of Set.Make (IdKey)
end

module IdMap : sig
  include module type of WrappedMap.Make (IdKey)
end

module UriKey : sig
  type t = documentUri

  val compare : t -> t -> int
end

module UriSet : sig
  include module type of Set.Make (UriKey)
end

module UriMap : sig
  include module type of WrappedMap.Make (UriKey)
end

val lsp_result_to_log_string : lsp_result -> string
