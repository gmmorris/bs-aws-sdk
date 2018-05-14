module AWS = {
  module S3 = {
    [@bs.deriving abstract]
    type putObjectRequest = {
      /**
               * The canned ACL to apply to the object.
               */ [@bs.as
                                                                    "ACL"
                                                                    ]
      accessControlPolicy:
        [@bs.string] [
          | [@bs.as "private"] `Private
          | [@bs.as "public-read"] `PublicRead
          | [@bs.as "public-read-write"] `PublicReadWrite
          | [@bs.as "authenticated-read"] `AuthenticatedRead
          | [@bs.as "aws-exec-read"] `AwsExecRead
          | [@bs.as "bucket-owner-read"] `BucketOwnerRead
          | [@bs.as "bucket-owner-full-control"] `BucketOwnerFullControl
        ],
      /**
               * Object data.
               */ [@bs.as "Body"]
      body: string,
      /**
               * Name of the bucket to which the PUT operation was initiated.
               */ [@bs.as
                                                                    "Bucket"
                                                                    ]
      bucket: string,
      /**
               * A standard MIME type describing the format of the object data.
               */ [@bs.as
                                                                    "ContentType"
                                                                    ]
      contentType: string,
      /**
               * Object key for which the PUT operation was initiated.
               */ [@bs.as
                                                                    "Key"
                                                                    ]
      key: string,
    };
    module ManagedUpload = {
      module SendData = {
        type t;
        /**
                 * URL of the uploaded object.
                 */
        [@bs.get]
        external location : t => string = "";
        /**
                 * ETag of the uploaded object.
                 */
        [@bs.get]
        external eTag : t => string = "";
        /**
                 * Bucket to which the object was uploaded.
                 */
        [@bs.get]
        external bucket : t => string = "";
        /**
                 * Key to which the object was uploaded.
                 */
        [@bs.get]
        external key : t => string = "";
      };
    };
    type t;
    [@bs.send] external upload : (t, putObjectRequest) => unit = "";
    [@bs.send]
    external uploadWithCallback :
      (
        t,
        putObjectRequest,
        (option(Js_exn.t), [@bs.obj] ManagedUpload.SendData.t) => unit
      ) =>
      unit =
      "upload";
  };
  [@bs.module "aws-sdk"] [@bs.new] external s3 : unit => S3.t = "S3";
};

let por =
  AWS.S3.putObjectRequest(
    ~accessControlPolicy=`Private,
    ~body="{\"blocked_creatives\":[{\"adid\":\"1234567890\"}]}",
    ~bucket="test-exchange-config-eu.unrulymedia.com",
    ~contentType="application/json",
    ~key="test/test.json",
  );

AWS.S3.uploadWithCallback(AWS.s3(), por, (e, data) =>
  switch (e) {
  | Some(err) =>
    Js.log(
      switch (Js.Exn.message(err)) {
      | Some(m) => m
      | None => "Weird error"
      },
    )
  | None =>
    Js.log(AWS.S3.ManagedUpload.SendData.location(data));
    Js.log(data);
  }
);