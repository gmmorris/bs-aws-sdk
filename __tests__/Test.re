open Jest;

open Rewire;

module Rewiring = {
  include MakeModuleRewiring(S3_upload.RewiredTestModule);
};

let rewiredModule =
  Rewiring.rewire(
    Node.Process.process##cwd() ++ "/lib/js/__tests__/S3_upload.bs.js",
  );

describe("S3", () =>
  testPromise("upload", () => {
    let expected = [%raw
      {|
        {
          "ACL": "private",
          "Body": "",
          "Bucket": "s3Bucket",
          "ContentType": "application/json",
          "Key": "key",
        }
      |}
    ];
    S3_upload.Mock.uploadWithOptions(rewiredModule, () =>
      S3_upload.RewiredTestModule.uploadWithOptions(rewiredModule) |> ignore
    )
    |> Js.Promise.(
         then_(options =>
           resolve(Expect.(expect(options) |> toEqual(expected)))
         )
       );
  })
);