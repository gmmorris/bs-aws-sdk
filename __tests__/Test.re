open Jest;

open MockAws;

let rewiredModule =
  MockAws.Rewiring.rewire(
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
    Js.Promise.make((~resolve, ~reject) => {
      let mockModule =
        AwsSdk.(
          aws(~_S3=() =>
            s3Instance(~upload=options =>
              resolve(. Expect.(expect(options) |> toEqual(expected)))
              |> ignore
            )
          )
        );
      mockAwsSdk(rewiredModule, mockModule, () =>
        RewiredTestModule.uploadWithOptions(rewiredModule)
      );
    });
  })
);